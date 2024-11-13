#include "./mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QPixmap pixmap(":/images/images/logo.png");
  QPixmap scaledPixmap =
      pixmap.scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  ui->imageLogo->setPixmap(scaledPixmap);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_selectVideoButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this,                                   // Parent
      tr("Select Video File"),                // Title
      QCoreApplication::applicationDirPath(), // Open Dialog Location
      tr("Video Files(*.mp4)")                // Filter
  );
  if (fileName.isNull() || fileName.isEmpty()) {
    ui->pathVideo->setText("Please select a valid file.");
    return;
  };
  ui->pathVideo->setText(fileName);
}

void MainWindow::on_selectImageButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this,                                   // Parent
      tr("Select an Image to Encrypt"),       // Title
      QCoreApplication::applicationDirPath(), // Open Dialog Location
      tr("Image Files (*.jpg *.jpeg *.png *.bmp *.avif)") // Filter
  );

  if (fileName.isEmpty()) {
    ui->pathImage->setText("Please select a valid file.");
    return;
  }

  ui->pathImage->setText(fileName);
}

void MainWindow::on_hideButton_clicked() {
  // controller.startEmbedding();
  QString pathVideo = ui->pathVideo->text();
  QString pathImage = ui->pathImage->text();

  if (pathImage.isEmpty() ||
      pathImage == "Please select a valid file." && pathVideo.isEmpty() ||
      pathVideo == "Please select a valid file.") {
    ui->resultsTitle->setText("Please select files to hide the image in audio");
    ui->resultsTitle->setStyleSheet("QLabel { color: rgb(185, 28, 28); }");
  } else {
    bool res = controller.startEmbedding(pathVideo.toStdString(),
                                         pathImage.toStdString());

    if (!res) {
      ui->resultsTitle->setText(
          "There was some problem, couldn't encrypted the image\n");
      ui->resultsTitle->setStyleSheet("QLabel { color: rgb(185, 28, 28); }");
    } else {
      ui->resultsTitle->setText("The image has been successfully encrypted\n");
      ui->resultsTitle->setStyleSheet("QLabel { color: rgb(74, 222, 128); }");
    }
  }
}

void MainWindow::on_retireveButton_clicked() {
  QString pathVideo = ui->pathVideo->text();
  QString pathImage = ui->pathImage->text();

  if (!pathImage.isEmpty() && pathImage != "Please select any file" &&
      !pathVideo.isEmpty() && pathVideo != "Please select any file") {
    controller.retrieveData(pathVideo.toStdString());
  }
}
