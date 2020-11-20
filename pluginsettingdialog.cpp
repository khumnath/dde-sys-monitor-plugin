#include "pluginsettingdialog.h"
#include "ui_pluginsettingdialog.h"

pluginSettingDialog::pluginSettingDialog(Settings *settings,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pluginSettingDialog)
{
    ui->setupUi(this);
    if(settings->value("chartModeCheckBox").toInt()){
        ui->tabWidget->setCurrentIndex(1);
    }
    else {
        ui->tabWidget->setCurrentIndex(0);
    }
    pal=QPalette();//首先初始化画板

    QMapIterator<QString,QVariant> i(*settings);
    while(i.hasNext())
    {
        i.next();
        QWidget *obj=findChild<QWidget*>(i.key());
        if(obj==0){qDebug()<<"Could not find object named："<<i.key();continue;}
        if(obj->metaObject()->className()==QStringLiteral("QComboBox"))
        {
            QComboBox *cb=(QComboBox*)obj;
            cb->setCurrentIndex(i.value().toInt());
        }
        else if(obj->metaObject()->className()==QStringLiteral("QCheckBox"))
        {
            QCheckBox *cb=(QCheckBox*)obj;
            cb->setChecked(i.value().toInt());
        }
        else if(obj->metaObject()->className()==QStringLiteral("QSpinBox"))
        {
            QSpinBox *sb=(QSpinBox*)obj;
            sb->setValue(i.value().toInt());
        }
        else if(obj->metaObject()->className()==QStringLiteral("QWidget"))
        {
            QWidget *wg=(QWidget*)obj;
            pal.setColor(QPalette::Background,i.value().value<QColor>());
            wg->setAutoFillBackground(true);
            wg->setPalette(pal);
            //qDebug()<<"Color is："<<i.value().value<QColor>();
        }
    }
    foreach(QPushButton* btn,findChildren<QPushButton*>(QRegExp("\\w*ColorPushButton")))
    {
        connect(btn,SIGNAL(clicked(bool)),this,SLOT(selectColor(void)));
    }
}

pluginSettingDialog::~pluginSettingDialog()
{
    delete ui;
}

void pluginSettingDialog::getDisplayContentSetting(Settings *settings)
{
    QMapIterator<QString,QVariant> i(*settings);
    while(i.hasNext())
    {
        i.next();
        QWidget *obj=findChild<QWidget*>(i.key());
        if(obj==0){qDebug()<<"Could not find object named："<<i.key();continue;}
        if(obj->metaObject()->className()==QStringLiteral("QComboBox"))
        {
            QComboBox *cb=(QComboBox*)obj;
            settings->insert(cb->objectName(),cb->currentIndex());
        }
        else if(obj->metaObject()->className()==QStringLiteral("QCheckBox"))
        {
            QCheckBox *cb=(QCheckBox*)obj;
            settings->insert(cb->objectName(),cb->isChecked());
        }
        else if(obj->metaObject()->className()==QStringLiteral("QSpinBox"))
        {
            QSpinBox *sb=(QSpinBox*)obj;
            settings->insert(sb->objectName(),sb->value());
        }
        else if(obj->metaObject()->className()==QStringLiteral("QWidget"))
        {
            QWidget *wg=(QWidget*)obj;
            settings->insert(wg->objectName(),wg->palette().background().color());
        }
    }
}

void pluginSettingDialog::selectColor()
{
    QWidget *colorWidget;
    //"netUpColorPushButton"-->"netUpWidget"
    colorWidget=findChild<QWidget*>(sender()->objectName().replace("ColorPushButton","Widget"));

    QColor color = QColorDialog::getColor(colorWidget->palette().background().color(),
                                          this,tr("Color dialog"),QColorDialog::ShowAlphaChannel);
    if(color!=QColor::Invalid)
    {
        pal.setColor(QPalette::Background,color);
        colorWidget->setAutoFillBackground(true);
        colorWidget->setPalette(pal);
    }
}
