#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
    QPixmap pixmap(":/images/images/logo.png");
    QPixmap scaledPixmap = pixmap.scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLogo->setPixmap(scaledPixmap);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_buttonSelectVideo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, // Parent
                                                    tr("Select Video File"), // Title
                                                    QCoreApplication::applicationDirPath(), // Open Dialog Location
                                                    tr("Video Files(*.mp4)") // Filter
                                                    );
    if (fileName.isNull() || fileName.isEmpty()) {
        qDebug() << "Please Select any File";
        return;
    };
    qDebug() << "FileName :: " << fileName;
    ui->pathVideo->setText(fileName);
}


void MainWindow::on_buttonSelectImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, // Parent
                                                    tr("Select Image File"), // Title
                                                    QCoreApplication::applicationDirPath(), // Open Dialog Location
                                                    tr("Image Files(*.jpg, *.png,  *.bmp)") // Filter
                                                    );
    if (fileName.isNull() || fileName.isEmpty()) {
        qDebug() << "Please Select any File";
        return;
    };
    qDebug() << "FileName :: " << fileName;
    ui->pathImage->setText(fileName);
}
