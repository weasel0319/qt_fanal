#include "game2048dialog.h"

game2048dialog::game2048dialog(QWidget *parent) : QDialog(parent)
{}
game2048dialog::game2048dialog(gamecheck ch , QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *hlay1 = new QHBoxLayout();
    QHBoxLayout *hlay2 = new QHBoxLayout();
    QVBoxLayout *vlay = new QVBoxLayout();

    switch (ch){
    case Over:
        setWindowTitle("Game Over");                    // gameover, undo 횟수 남아있을 시 다이얼로그
        lbl = new QLabel("메뉴를 선택하세요");
        newbtn = new QPushButton("New Game");
        undobtn = new QPushButton("Undo");
        backbtn = new QPushButton("메뉴로 돌아가기");

        hlay1->addWidget(lbl);

        hlay2->addWidget(newbtn);
        hlay2->addWidget(undobtn);
        hlay2->addWidget(backbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(newbtn,&QPushButton::clicked,[=](){slot_newbtn();});
        connect(undobtn,&QPushButton::clicked,[=](){slot_undobtn();});
        connect(backbtn,&QPushButton::clicked,[=](){slot_mainbtn();});
        break;
    case Noundo:
        setWindowTitle("Game Over");                    // gameover, undo 횟수 없을 시 다이얼로그
        lbls = new QLabel("undo 기회 없음");
        lbl = new QLabel("메뉴를 선택하세요");
        newbtn = new QPushButton("New Game");
        backbtn = new QPushButton("메뉴로 돌아가기");

        hlay1->addWidget(lbls);
        hlay1->addWidget(lbl);

        hlay2->addWidget(newbtn);
        hlay2->addWidget(backbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(newbtn,&QPushButton::clicked,[=](){slot_newbtn();});
        connect(backbtn,&QPushButton::clicked,[=](){slot_mainbtn();});
        break;
    }


}


game2048dialog::game2048dialog(qint32 score, qint32 maxscore, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Game Clear");                       // game clear시 다이얼로그

    lbls = new QLabel("score: " + QString::number(score) + "!");
    lblm = new QLabel("당신의 최고 점수: " + QString::number(maxscore));
    lbl = new QLabel("메뉴를 선택하세요");

    newbtn = new QPushButton("New Game");
    undobtn = new QPushButton("Undo");
    backbtn = new QPushButton("메뉴로 돌아가기");

    QHBoxLayout *hlay1 = new QHBoxLayout();
    hlay1->addWidget(lbls);
    hlay1->addWidget(lblm);
    hlay1->addWidget(lbl);

    QHBoxLayout *hlay2 = new QHBoxLayout();
    hlay2->addWidget(newbtn);
    hlay2->addWidget(backbtn);

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);
    setLayout(vlay);

    connect(newbtn,&QPushButton::clicked,[=](){slot_newbtn();});
    connect(backbtn,&QPushButton::clicked,[=](){slot_mainbtn();});
}
void game2048dialog::slot_newbtn()
{
    emit newGame();
    close();
}

void game2048dialog::slot_undobtn()
{
    emit undoGame();
    close();
}

void game2048dialog::slot_mainbtn()
{
    emit returnMenu();
    close();
}

game2048dialog::~game2048dialog()
{
}
