#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./../controller/stegoController.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_selectVideoButton_clicked();
  void on_selectImageButton_clicked();

  void on_hideButton_clicked();

  void on_retireveButton_clicked();

  private:
  Ui::MainWindow *ui;
  StegoController controller;
};
#endif // MAINWINDOW_H
