#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->contactBook->setColumnCount(4);
    QStringList colTotal;
    colTotal << "學號" << "班級" << "姓名" << "電話";
    ui->contactBook->setHorizontalHeaderLabels(colTotal);
}

Widget::~Widget()
{
    delete ui;
}

// 1. 新增按鈕
void Widget::on_insertButton_clicked()
{
    QTableWidgetItem *col1 = new QTableWidgetItem(ui->lineEdit->text());
    QTableWidgetItem *col2 = new QTableWidgetItem(ui->lineEdit_2->text());
    QTableWidgetItem *col3 = new QTableWidgetItem(ui->lineEdit_3->text());
    QTableWidgetItem *col4 = new QTableWidgetItem(ui->lineEdit_4->text());

    int row = ui->contactBook->rowCount();
    ui->contactBook->insertRow(row);

    ui->contactBook->setItem(row, 0, col1);
    ui->contactBook->setItem(row, 1, col2);
    ui->contactBook->setItem(row, 2, col3);
    ui->contactBook->setItem(row, 3, col4);
}

// 2. 寫入檔案函式 (Qt 6 修正版)
void Write(QString Filename, QString str)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out.setEncoding(QStringConverter::Utf8); // 💡 Qt 6 的新寫法
    out << str;
    mFile.flush();
    mFile.close();
}

// 3. 匯出按鈕
void Widget::on_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "選擇儲存位置", "", "csv(*.csv);;text(*.txt)");

    if (!fileName.isEmpty()) {
        QString saveStr;

        for (int i = 0; i < ui->contactBook->rowCount(); ++i) {
            QStringList rowData;
            for (int j = 0; j < ui->contactBook->columnCount(); ++j) {
                QTableWidgetItem *item = ui->contactBook->item(i, j);
                rowData << (item ? item->text() : "");
            }
            saveStr += rowData.join(",") + "\n";
        }

        Write(fileName, saveStr);
        QMessageBox::information(this, "成功", "資料已成功匯出為 CSV 檔！");
    }
}

// 4. 匯入按鈕 (Qt 6 修正版)
void Widget::on_importButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "選擇匯入的 CSV 檔案", "", "CSV 檔案 (*.csv);;文字檔案 (*.txt)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile mFile(fileName);
    if (!mFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "錯誤", "無法開啟此檔案！");
        return;
    }

    ui->contactBook->setRowCount(0); // 清空舊資料

    QTextStream in(&mFile);
    in.setEncoding(QStringConverter::Utf8); // 💡 Qt 6 的新寫法
    // 💡 備註：如果之後發現 Excel 存出來的中文變亂碼，可以把上面這一行改成：
    // in.setEncoding(QStringConverter::System); (會自動抓你作業系統的預設編碼，例如 Big5)

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.trimmed().isEmpty()) continue;

        QStringList fields = line.split(",");

        if (fields.size() >= 4) {
            int currentRow = ui->contactBook->rowCount();
            ui->contactBook->insertRow(currentRow);

            ui->contactBook->setItem(currentRow, 0, new QTableWidgetItem(fields[0].trimmed()));
            ui->contactBook->setItem(currentRow, 1, new QTableWidgetItem(fields[1].trimmed()));
            ui->contactBook->setItem(currentRow, 2, new QTableWidgetItem(fields[2].trimmed()));
            ui->contactBook->setItem(currentRow, 3, new QTableWidgetItem(fields[3].trimmed()));
        }
    }

    mFile.close();
    QMessageBox::information(this, "成功", "CSV 資料已成功匯入表格！");
}
