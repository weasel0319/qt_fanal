#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class profileDialog :public QDialog
{
    Q_OBJECT
public:
    enum Mode {             // 다이어그램 종류 구분
        Create,
        Move,
        Delete,
        Search,
        Noname,
        Edit
    };


    profileDialog(QWidget *parent = 0);             // 매개변수에 따라 다른 다이얼로그를 생성함
    profileDialog(Mode i,QWidget *parent = 0);


    ~profileDialog();
private:
    QLabel *name;               // 프로필 생성 다이어그램
    QLineEdit *nameedit;
    QPushButton *createbtn;
    QPushButton *cancelbtn;

    QLabel *info;               // 프로필로 이동시키는 다이어그램
    QPushButton *movebtn;

    QPushButton *delbtn;        // 프로필 삭제용

    QPushButton *searchbtn;     // 프로필 검색용
    QLineEdit *searchedit;

    QLineEdit *editprofileedit;  // 프로필 수정용
    QPushButton *editbtn;

    qint32 cnt = 0;
private slots:
    void slot_createbtn();      // 프로필 생성
    void slot_cancelbtn();

    void slot_move();           // 프로필로 이동

    void slot_del();            // 프로필 삭제

    void slot_search();         // 프로필 검색

    void slot_edit();
signals:
    void sendname(QString);     // 생성된 이름 전달

    void movesig();             // 움직이는 신호 발생

    void delsig();              // 삭제 신호 발생

    void searchsig(QString);    // 프로필 검색 신호

    void editsig(QString);      // 프로필 수정 신호
};

#endif // PROFILEDIALOG_H
