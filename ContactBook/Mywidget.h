#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_insertButton_clicked();
    void on_exportButton_clicked();
    void on_importButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // MYWIDGET_H
