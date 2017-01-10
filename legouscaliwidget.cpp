#include "legouscaliwidget.h"
#include "ui_legouscaliwidget.h"
#include "application.h"
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <QMessageBox>
#include <string.h>


LegoUSCaliWidget::LegoUSCaliWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LegoUSCaliWidget)
{
    ui->setupUi(this);
    m_numberOfFiles = 0;
    setWindowTitle( "Lego US calibration" );

    //find mincconvert
    QFile converter("/usr/bin/mincconvert");
    if( !converter.exists() )
    {
        converter.setFileName("/usr/local/bin/mincconvert");
        if( !converter.exists() )
        {
            converter.setFileName("/usr/local/minc/bin/mincconvert");
            if( !converter.exists() )
            {
                converter.setFileName("/usr/local/mni/minc/bin/mincconvert");
                if( !converter.exists() )
                {
                    converter.setFileName("/opt/minc/bin/mincconvert");
                }
            }
        }
    }
    if( converter.exists() )
    {
        QFileInfo fi1(converter);
        if( fi1.isExecutable() )
        {
            m_mincconvert.append(fi1.absoluteFilePath());
            m_mincconvert.append( " -clobber ");
        }
        else
        {
            QMessageBox::critical( 0, "Error", "mincconvert not found in standars locations not excutable", 1, 0 );
        }
    }
    else
    {
        QMessageBox::critical( 0, "Error", "mincconvert not found in standars locations", 1, 0 );
    }


    //find mincmakescale
    QFile makescalar("/usr/bin/mincmakescalar");
    if( !makescalar.exists() )
    {
        makescalar.setFileName("/usr/local/bin/mincmakescalar");
        if( !makescalar.exists() )
        {
            makescalar.setFileName("/usr/local/minc/bin/mincmakescalar");
            if( !makescalar.exists() )
            {
                makescalar.setFileName("/usr/local/mni/minc/bin/mincmakescalar");
                if( !makescalar.exists() )
                {
                    makescalar.setFileName("/opt/minc/bin/mincmakescalar");
                }
            }
        }
    }
    if( makescalar.exists() )
    {
        QFileInfo fi2(makescalar);
        if( fi2.isExecutable() )
        {
            m_mincmakescale.append(fi2.absoluteFilePath());
            m_mincmakescale.append( " -rgbtogray -short -clobber ");
        }
        else
        {
            QMessageBox::critical( 0, "Error", "mincmakescalar not found in standars locations, not executable", 1, 0 );
        }
    }
    else
    {
        QMessageBox::critical( 0, "Error", "mincmakescalar not found in standars locations", 1, 0 );
    }


    // find minctracc, should be built in a directory
    QFile tracc("/Users/yimingxiao/Documents/programming/MINC-UScali/build/minctracc/minctracc");
    if( tracc.exists() )
    {
        QFileInfo fi3(tracc);
        if( fi3.isExecutable() )
        {
            m_minctracc.append(fi3.absoluteFilePath());
            m_minctracc.append( " -est_center -lsq7 -xcorr -slices_to_volume -trilinear -source_lattice -tol 0.0001 -threshold 0 -1 -clobber ");
        }
        else
        {
            QMessageBox::critical( 0, "Error", "minctracc for calibration not found in the designated location, not excutable", 1, 0 );
        }
    }
    else
    {
        QMessageBox::critical( 0, "Error", "minctracc for calibration not found in the designated location", 1, 0 );
    }


    // check if the phantom image is in palce
    QFile phantom("/Users/yimingxiao/Documents/programming/MINC-UScali/build/minctracc/minctracc");
    if( phantom.exists())
    {
      QFileInfo fi4(phantom);
      m_phantom.append(fi4.absoluteFilePath());
    }
    else
    {
       QMessageBox::critical( 0, "Error", "phanotm image is not in place", 1, 0 );
    }



    ui->SimplexComboBox->clear(); // need to check
    ui->StepComboBox->clear();
    ui->samplerateComboBox->clear();

    // put the content to the Comboboxes
    ui->SimplexComboBox->addItem( "3", QVariant( "3" ) );
    ui->SimplexComboBox->addItem( "4", QVariant( "4" ) );
    ui->SimplexComboBox->addItem( "5", QVariant( "5" ) );
    ui->StepComboBox->setCurrentIndex( 1 );

    ui->StepComboBox->addItem( "1x1x1", QVariant( "1 1 1" ) );
    ui->StepComboBox->addItem( "2x2x2", QVariant( "2 2 2" ) );
    ui->StepComboBox->addItem( "4x4x4", QVariant( "4 4 4" ) );
    ui->StepComboBox->addItem( "8x8x8", QVariant( "8 8 8" ) );
    ui->StepComboBox->setCurrentIndex( 2 );

    // sampling rate
    ui->samplerateComboBox->addItem( "Full", QVariant( "{0,1,2,3,4,5,6,7,8,9}" ) );
    ui->samplerateComboBox->addItem( "1/2", QVariant( "{0,2,4,6,8}" ) );
    ui->samplerateComboBox->addItem( "1/5", QVariant( "{5,0}" ) );
    ui->samplerateComboBox->addItem( "1/10", QVariant( "{0}" ) );
    ui->samplerateComboBox->setCurrentIndex( 1 );


    UpdateUi();

}

LegoUSCaliWidget::~LegoUSCaliWidget()
{
    delete ui;
}

// set the input names
void LegoUSCaliWidget::on_acqPushButton_clicked()
{

    QString extension( ".mnc" );
    bool success = Application::GetInstance().GetOpenFileSequence( m_inFiles, extension, "Select first file of acquisition", QDir::homePath(), "Minc file (*.mnc)" );
    if (success)
    {
        m_numberOfFiles = m_inFiles.count();

        QFileInfo info( m_inFiles[0] );
        m_inputDirectory = info.dir().absolutePath();
    }


    UpdateUi();
}


// load the initialization calibration matrix
void LegoUSCaliWidget::on_initxfmPushButton_clicked()
{

    QString xfmname = Application::GetInstance().GetOpenFileName( "Select initial calibration", QDir::homePath(), "xfm file (*.xfm)" );
    if( !xfmname.isNull() )
    {
        m_initialxfm = xfmname;
    }

    UpdateUi();
}


// select the output directory
void LegoUSCaliWidget::on_outputPushButton_clicked()
{
    m_outputDirectory = QFileDialog::getExistingDirectory( this, "Output Folder", m_inputDirectory );
    UpdateUi();
}



// when all are reday to run
void LegoUSCaliWidget::on_startButton_clicked()
{
    QFileInfo info( m_outputDirectory );
    if( !info.isWritable() )
    {
        QString msg( m_outputDirectory );
        msg.append( " is not writeable." );
        QMessageBox::warning( 0, "Error", msg );
        return;
    }


    //get the parameter from the interfaces
    QString simplexparam = ui->SimplexComboBox->itemData( ui->SimplexComboBox->currentIndex() ).toString();
    QString stepsize = ui->StepComboBox->itemData( ui->StepComboBox->currentIndex() ).toString();
    QString samplingrate = ui->samplerateComboBox->itemData( ui->samplerateComboBox->currentIndex() ).toString();
    QString filename = m_inFiles.at(1).split('.').first();



    ui->startButton->setEnabled( false );

    //make folders for the ouputs
    QString command("mkdir");
    command.append( "\"" + m_outputDirectory + "/" + "grey " + "\"" + m_outputDirectory + "/" + "minc1 " + m_outputDirectory + "/" + "calibrationxfm ");
    system( command.toUtf8().data() );
    command.clear();

    // go through processing
    for( int i = 0; i < m_numberOfFiles; i++ )
    {

        // 1. do rgb to grey
        QString command1(m_mincmakescale);
        command1.append( "\"" + m_inFiles.at(i) + "\"" + m_outputDirectory + "/grey/grey" + m_inFiles.at(i));
        int ret = system( command1.toUtf8().data() );
        if( ret < 0 )
            QMessageBox::warning( 0, "Error", "There were errors converting rgb to greyscale." );
        command1.clear();

        // 2. convert minc2 to minc1
        QString command2(m_mincconvert);
        command2.append( "\"" + m_outputDirectory + "/grey/grey" + m_inFiles.at(i) + "\"" + m_outputDirectory + "/minc1/minc1" + m_inFiles.at(i));
        int ret2 = system( command2.toUtf8().data() );
        if( ret2 < 0 )
            QMessageBox::warning( 0, "Error", "There were errors converting to minc1." );
        command2.clear();

        // 3. finish up the registration
        QString command3(m_minctracc);
        command3.append( "- simplex" + simplexparam + "\"" + "-step" + "\"" + stepsize + "\"" + "-transformation" + "\"" + m_initialxfm );
        command3.append( "\"" + m_inputDirectory + "/" + filename + "0???" + samplingrate + "\"" + m_phantom + "\"" + m_outputDirectory + "/calibrationxfm/Lego-US-cali.xfm");
        int ret3 = system( command3.toUtf8().data() );
        if( ret3 < 0 )
            QMessageBox::warning( 0, "Error", "Something wrong with the registration." );
        command3.clear();

    }


    ui->startButton->setEnabled( true );

}



void LegoUSCaliWidget::UpdateUi()
{

    // if any of the mictools is not found
    if( m_mincconvert.isEmpty() || m_mincmakescale.isEmpty() || m_minctracc.isEmpty() || m_phantom.isEmpty())
    {
        ui->fileLineEdit->setEnabled( false );
        ui->initxfmLineEdit->setEnabled( false );
        ui->outputLineEdit->setEnabled( false );
        ui->acqPushButton->setEnabled( false );
        ui->initxfmPushButton->setEnabled( false );
        ui->outputPushButton->setEnabled( false );
        ui->startButton->setEnabled( false );
        ui->SimplexComboBox->setEnabled( false );
        ui->StepComboBox->setEnabled( false );
        ui->samplerateComboBox->setEnabled( false );


        return;
    }

    // if there is no US image in the folder
    if( m_numberOfFiles == 0 )
    {
        ui->outputLineEdit->blockSignals( true );
        ui->outputLineEdit->clear();
        ui->outputLineEdit->blockSignals( false );
        ui->startButton->setEnabled( false );

        return;
    }

    ui->fileLineEdit->blockSignals( true );
    if(!m_inFiles.isEmpty())
    {
        ui->fileLineEdit->setText( m_inputDirectory );
        ui->fileLineEdit->blockSignals( false );
    }

    ui->initxfmLineEdit->blockSignals( true );
    if(!m_initialxfm.isEmpty())
    {
        ui->initxfmLineEdit->setText(m_initialxfm);
        ui->initxfmLineEdit->blockSignals( false );
    }

    // if there is no output info provided
    ui->outputLineEdit->blockSignals( true );
    if(!m_outputDirectory.isEmpty())
    {
        if( m_outputDirectory == m_inputDirectory  &&  m_numberOfFiles > 1 )
        {
            QMessageBox::warning( 0, "Error", "Output Folder may not be the same as Input Folder" );
        }
        else
        {
            ui->outputLineEdit->setText( m_outputDirectory );

        }
        ui->outputLineEdit->blockSignals( false );
    }


    if( !m_inFiles.isEmpty() && !m_initialxfm.isEmpty() && !m_outputDirectory.isEmpty() )
    {
        ui->startButton->setEnabled( true );
    }
    else
    {
        ui->startButton->setEnabled( false );
    }





}
