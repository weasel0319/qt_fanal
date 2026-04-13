#ifndef GAME2_48DIALOG_H
#define GAME2_48DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
class game2048dialog :public QDialog
{
    Q_OBJECT
public:
    enum gamecheck {                                                    // 구분용 enum
        Over,
        Noundo
    };

    game2048dialog(QWidget *parent = 0);                                // 매개변수에 따라 다르게 동작
    game2048dialog(gamecheck ch, QWidget *parent = 0);
    game2048dialog(qint32 score, qint32 maxscore, QWidget *parent = 0);

    ~game2048dialog();
private:
    QLabel *lbl;                                                        // 메세지 표시용
    QLabel *lbls;
    QLabel *lblm;

    QPushButton *newbtn;                                                // 상황에 따라 다른 버튼 사용
    QPushButton *undobtn;
    QPushButton *backbtn;
public slots:
    void slot_newbtn();                                                 // 누른 버튼에 해당하는 슬롯 함수
    void slot_undobtn();
    void slot_mainbtn();
signals:
    void newGame();                                                     // 슬롯함수에서 발생시킬 시그널
    void undoGame();
    void returnMenu();
};

#endif // GAME2_48DIALOG_H
