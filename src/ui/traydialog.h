#ifndef AMIXST_TRAYDIALOG_H
#define AMIXST_TRAYDIALOG_H

#include <QDialog>

namespace AMixST {

namespace Ui {
class TrayDialog;
}

class TrayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrayDialog(QWidget *parent = 0);
    ~TrayDialog();
    void show();
    void Relocate();

protected:
    void focusOutEvent(QFocusEvent* event);

private:
    Ui::TrayDialog *ui;
};


} // namespace AMixST
#endif // AMIXST_TRAYDIALOG_H
