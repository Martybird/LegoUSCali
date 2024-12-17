#ifndef LEGOUSCALIWIDGET_H
#define LEGOUSCALIWIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>


class Application;

namespace Ui
{
    class LegoUSCaliWidget;
}


class LegoUSCaliWidget : public QWidget
{

    Q_OBJECT

public:

    explicit LegoUSCaliWidget(QWidget *parent = 0);
    ~LegoUSCaliWidget();



private:

    void UpdateUi();

    Ui::LegoUSCaliWidget * ui;

    QStringList m_inFiles;
    QString m_inputDirectory;
    QString m_outputDirectory;
    QString m_initialxfm;

    int m_numberOfFiles;


private slots:

    void on_acqPushButton_clicked();
    void on_initxfmPushButton_clicked();
    void on_outputPushButton_clicked();
    void on_startButton_clicked();


protected:
    // Path to minctools
    QString m_mincconvert;
    QString m_mincmakescale;
    QString m_minctracc;
    QString m_phantom;

};

#endif
