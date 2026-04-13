#include "profiledialog.h"
profileDialog::profileDialog(QWidget *parent)
    : QDialog(parent)
{}
profileDialog::profileDialog(Mode i, QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout *hlay1 = new QHBoxLayout();
    QHBoxLayout *hlay2 = new QHBoxLayout();
    QVBoxLayout *vlay = new QVBoxLayout();


    switch(i){
    case Create:
        setWindowTitle("프로필 생성");
        name = new QLabel("이름");
        nameedit = new QLineEdit("");

        createbtn = new QPushButton("생성");
        cancelbtn = new QPushButton("취소");

        hlay1->addWidget(name);
        hlay1->addWidget(nameedit);

        hlay2->addWidget(createbtn);
        hlay2->addWidget(cancelbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(createbtn, &QPushButton::clicked, this, &profileDialog::slot_createbtn);
        connect(cancelbtn, &QPushButton::clicked, this, &profileDialog::slot_cancelbtn);
        break;
    case Move:
        setWindowTitle("프로필 없음");
        info = new QLabel("프로필을 먼저 생성해야 합니다.\n프로필로 이동하시겠습니까?");

        movebtn = new QPushButton("이동");
        cancelbtn = new QPushButton("취소");

        hlay1->addWidget(info);

        hlay2->addWidget(movebtn);
        hlay2->addWidget(cancelbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(movebtn, &QPushButton::clicked, this, &profileDialog::slot_move);
        connect(cancelbtn, &QPushButton::clicked, this, &profileDialog::slot_cancelbtn);
        break;
    case Delete:
        setWindowTitle("프로필 삭제");
        info = new QLabel("프로필을 삭제하시겠습니까?");

        delbtn = new QPushButton("삭제");
        cancelbtn = new QPushButton("취소");

        hlay1->addWidget(info);

        hlay2->addWidget(delbtn);
        hlay2->addWidget(cancelbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(delbtn, &QPushButton::clicked, this, &profileDialog::slot_del);
        connect(cancelbtn, &QPushButton::clicked, this, &profileDialog::slot_cancelbtn);
        break;
    case Search:
        setWindowTitle("프로필 검색");
        name = new QLabel("검색할 이름");
        searchedit = new QLineEdit("");

        searchbtn = new QPushButton("검색");
        cancelbtn = new QPushButton("취소");

        hlay1->addWidget(name);
        hlay1->addWidget(searchedit);

        hlay2->addWidget(searchbtn);
        hlay2->addWidget(cancelbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(searchbtn, &QPushButton::clicked, this, &profileDialog::slot_search);
        connect(cancelbtn, &QPushButton::clicked, this, &profileDialog::slot_cancelbtn);
        break;
    case Noname:
        setWindowTitle("해당 이름 없음");
        info = new QLabel("검색어에 해당되는 사람 없음");
        cancelbtn = new QPushButton("확인");

        hlay1->addWidget(info);
        hlay2->addWidget(cancelbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);

        setLayout(vlay);

        connect(cancelbtn, &QPushButton::clicked, this, &profileDialog::slot_cancelbtn);
        break;
    case Edit:
        setWindowTitle("프로필 수정");
        name = new QLabel("변경할 이름");
        editprofileedit = new QLineEdit("");

        editbtn = new QPushButton("검색");
        cancelbtn = new QPushButton("취소");

        hlay1->addWidget(name);
        hlay1->addWidget(editprofileedit);

        hlay2->addWidget(editbtn);
        hlay2->addWidget(cancelbtn);

        vlay->addLayout(hlay1);
        vlay->addLayout(hlay2);
        setLayout(vlay);

        connect(editbtn, &QPushButton::clicked, this, &profileDialog::slot_edit);
        connect(cancelbtn, &QPushButton::clicked, this, &profileDialog::slot_cancelbtn);
        break;
    }
}


// 프로필 생성용 함수
void profileDialog::slot_createbtn()
{
    emit sendname(nameedit->text());
    close();
}

void profileDialog::slot_cancelbtn()
{
    reject();
}
// 프로필로 이동용 함수
void profileDialog::slot_move(){
    emit movesig();
    close();
}
// 프로필 삭제용 함수
void profileDialog::slot_del(){
    emit delsig();
    close();
}
void profileDialog::slot_search(){
    emit searchsig(searchedit->text());
    close();
}
void profileDialog::slot_edit(){
    emit editsig(editprofileedit->text());
    close();
}
profileDialog::~profileDialog() {}
