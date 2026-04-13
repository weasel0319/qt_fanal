#include "mainwidget.h"

#include "./ui_mainwidget.h"
#include "profiledialog.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->mainStWidget->setCurrentWidget(ui->page_1_StartMenu);   // 처음 시작 화면 고정
    ui->playGameStWidget->setCurrentWidget(ui->page_1menu);

    connect(ui->gameSelectBt,&QPushButton::clicked,[&](){
        if (ui->listWidgetProfile->currentItem()){                  // 현재 선택된 프로필이 있을 시
            ui->mainStWidget->setCurrentWidget(ui->page_2_GameMenu);// stackwidget 변경
        }
        else{                                                       // 없는 경우 다이얼로그로
            profileDialog moveProfile(profileDialog::Move);                           // 프로필 생성으로 이동
            connect(&moveProfile,&profileDialog::movesig,[&](){ openProfile(); });
            moveProfile.exec();
        }
    });

    connect(ui->backMainBt,&QPushButton::clicked,[&](){ui->mainStWidget->setCurrentWidget(ui->page_1_StartMenu);});
    // 돌아가기 버튼 누르면 화면 전환

    // 디자인 영역
    ui->ProfilesearchBt->setIcon(QIcon(":/resourceMain/resourceMain/q.png"));                                              // 프로필 검색 아이콘
    ui->ProfilesearchBt->setText("프로필 검색");

    QPixmap veda, pad1, pad2;                                                   // 메인 화면 이미지                                    // 메인화면 사진 삽입
    veda.load(":/resourceMain/resourceMain/veda.png");
    pad1.load(":/resourceMain/resourceMain/pad.png");
    pad2.load(":/resourceMain/resourceMain/pad2.png");
    ui->vedalabel->setPixmap(veda.scaled(ui->vedalabel->width(),ui->vedalabel->height(), Qt::KeepAspectRatio));
    ui->pad1label->setPixmap(pad1.scaled(ui->pad1label->width(),ui->pad1label->height(), Qt::KeepAspectRatio));
    ui->pad2label->setPixmap(pad2.scaled(ui->pad2label->width(),ui->pad2label->height(), Qt::KeepAspectRatio));

    // page 1 디자인
    ui->page_1menu->setStyleSheet(" background-color: rgb(244, 115, 31)");

    ui->gameSelectBt->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(255, 60, 60);" // 배경색
        "color: white;"
        "font-weight: bold;"
        "border: 2px outset rgb(200, 40, 40);" // 튀어나온 듯한 효과를 위한 테두리
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        // 버튼을 눌렀을 때의 스타일 (입체감 있는 버튼의 핵심)
        "QPushButton:pressed {"
        "background-color: rgb(200, 40, 40);" // 배경색을 더 어둡게
        "border: 2px inset rgb(200, 40, 40);"  // 테두리를 안으로 들어간 듯한 효과로 변경
        "}"
        );
    ui->profileBt->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(70, 140, 200);" // 톤 다운된 파란색 배경
        "color: white;"                      // 흰색 글자
        "font-weight: bold;"
        "border: 2px outset rgb(50, 100, 160);" // 튀어나온 듯한 효과를 위한 테두리
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        // 버튼을 눌렀을 때 (pressed) 스타일
        "QPushButton:pressed {"
        "background-color: rgb(50, 100, 160);" // 배경색을 더 어둡게
        "border: 2px inset rgb(50, 100, 160);"  // 테두리를 안으로 들어간 듯한 효과로 변경
        "}"
        );
    ui->exitBt->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(0, 128, 0);" // 짙은 초록색 배경
        "color: white;"                   // 흰색 글자
        "font-weight: bold;"
        "border: 2px outset rgb(0, 80, 0);" // 튀어나온 듯한 효과를 위한 테두리
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        // 버튼을 눌렀을 때 (pressed) 스타일
        "QPushButton:pressed {"
        "background-color: rgb(0, 80, 0);" // 배경색을 더 어둡게
        "border: 2px inset rgb(0, 80, 0);"  // 테두리를 안으로 들어간 듯한 효과로 변경
        "}"
        );
    ui->game2048Bt->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(255, 60, 60);" // 배경색
        "color: white;"
        "font-weight: bold;"
        "border: 2px outset rgb(200, 40, 40);" // 튀어나온 듯한 효과를 위한 테두리
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        // 버튼을 눌렀을 때의 스타일 (입체감 있는 버튼의 핵심)
        "QPushButton:pressed {"
        "background-color: rgb(200, 40, 40);" // 배경색을 더 어둡게
        "border: 2px inset rgb(200, 40, 40);"  // 테두리를 안으로 들어간 듯한 효과로 변경
        "}"
        );
    ui->gameyatzeeBt->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(70, 140, 200);" // 톤 다운된 파란색 배경
        "color: white;"                      // 흰색 글자
        "font-weight: bold;"
        "border: 2px outset rgb(50, 100, 160);" // 튀어나온 듯한 효과를 위한 테두리
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        // 버튼을 눌렀을 때 (pressed) 스타일
        "QPushButton:pressed {"
        "background-color: rgb(50, 100, 160);" // 배경색을 더 어둡게
        "border: 2px inset rgb(50, 100, 160);"  // 테두리를 안으로 들어간 듯한 효과로 변경
        "}"
        );
    ui->backMainBt->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(0, 128, 0);" // 짙은 초록색 배경
        "color: white;"                   // 흰색 글자
        "font-weight: bold;"
        "border: 2px outset rgb(0, 80, 0);" // 튀어나온 듯한 효과를 위한 테두리
        "border-radius: 5px;"
        "padding: 5px;"
        "}"
        // 버튼을 눌렀을 때 (pressed) 스타일
        "QPushButton:pressed {"
        "background-color: rgb(0, 80, 0);" // 배경색을 더 어둡게
        "border: 2px inset rgb(0, 80, 0);"  // 테두리를 안으로 들어간 듯한 효과로 변경
        "}"
        );

    // page 2 디자인
    ui->page_2profile->setStyleSheet(
        "#page_2profile { "
        "background-color: rgb(244, 115, 31);" // 오렌지색 배경색
        "}"

        "QListWidget { background-color: transparent; }" // 리스트 위젯의 배경을 투명하게
    );
    ui->ProfilemakeBt->setStyleSheet(               // 버튼 색상
        " background-color: rgb(220, 95, 20);"
        " color: rgb(0, 0, 0);");
    ui->ProfiledelBt->setStyleSheet(
        " background-color: rgb(220, 95, 20);"
        " color: rgb(0, 0, 0);");
    ui->ProfilebackBt->setStyleSheet(
        " background-color: rgb(220, 95, 20);"
        " color: rgb(0, 0, 0);");
    ui->ProfilesearchBt->setStyleSheet(
        " background-color: rgb(220, 95, 20);"
        " color: rgb(0, 0, 0);");
    ui->ProfileeditBt->setStyleSheet(
        " background-color: rgb(220, 95, 20);"
        " color: rgb(0, 0, 0);");


    // 파일 불러오기(프로그램 실행 시 불러옴)
    QFile file("SaveList.txt");

    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        qint32 cnt = 0;


        while(1){
            QString str;
            if(in.atEnd()) {
                break;
            }

            QListWidgetItem* checkitem = new QListWidgetItem(ui->listWidgetProfile);
            QWidget *containerWidget = new QWidget();                   // 여러 개의 widget합치기용
            QHBoxLayout* hLayout = new QHBoxLayout(containerWidget);    // widget 공간에 hlayout 형태로 생성
            QLabel* view1;          // 불러온 값 저장용
            QLabel* profileName;
            QLabel* view2;
            QLabel* profileYa;
            QLabel* view3;
            QLabel* profile2048;

            for (int i = 0; i < 3; i++){            // 이름 점수 점수 형태로 저장되어있는 파일을 파싱
                in >> str;
                switch(i){
                case 0:
                    view1 = new QLabel("이름: ");
                    profileName = new QLabel(str);
                    hLayout->addWidget(view1);
                    hLayout->addWidget(profileName);
                    break;
                case 1:
                    view2 = new QLabel("Yacht 최고 점수: ");
                    profileYa = new QLabel(str);
                    hLayout->addWidget(view2);
                    hLayout->addWidget(profileYa);
                    break;
                case 2:
                    view3 = new QLabel("2048 최고 점수: ");
                    profile2048 = new QLabel(str);
                    hLayout->addWidget(view3);
                    hLayout->addWidget(profile2048);
                    break;
                }
            }
            checkitem->setSizeHint(containerWidget->sizeHint());        // 시스템 자동으로 크기 배열
            ui->listWidgetProfile->setItemWidget(checkitem,containerWidget);
        }
        ui->listWidgetProfile->setCurrentRow(0);            // 프로필 로드 후 자동으로 맨 위 대표 프로필 선택
        file.close();                                       // 파일 닫기
    }
}

MainWidget::~MainWidget()
{
    delete ui;
}

// 2048 실행
void MainWidget::open2048(){
    QListWidgetItem* nowProfile = ui->listWidgetProfile->currentItem();                 // 프로필의 선택된 리스트 가져옴
    QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfile);           //
    QHBoxLayout*hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());        // 프로필 컨테이너 순서대로 변환
    QLabel* MaxLabel = qobject_cast<QLabel*>(hLayout->itemAt(5)->widget());             // 형변환을 통해 값 추출
    qint32 max = MaxLabel->text().QString::toInt();                     // 현재 저장되어 있는 해당 게임 max값 추출
    play2048 = new game2048(max,this);                                  // 생성자로 max값 전달
    ui->playGameStWidget->addWidget(play2048);                          // 화면 전환
    ui->playGameStWidget->setCurrentWidget(play2048);
}

void MainWidget::openyatzee(){
    QListWidgetItem* nowProfile = ui->listWidgetProfile->currentItem();                 // 프로필의 선택된 리스트 가져옴
    QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfile);           //
    QHBoxLayout*hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());        // 프로필 컨테이너 순서대로 변환
    QLabel* MaxLabel = qobject_cast<QLabel*>(hLayout->itemAt(3)->widget());             // 형변환을 통해 값 추출
    qint32 max = MaxLabel->text().QString::toInt();                     // 현재 저장되어 있는 해당 게임 max값 추출
    playYatzee = new Yatzee(max,this);                                  // 생성자로 max값 전달
    connect(playYatzee, &Yatzee::backToMenu, this, &MainWidget::returnMenuyatzee);       // yatzee가 보내는 시그널과 mainwidget의 술롯 연결
    ui->playGameStWidget->addWidget(playYatzee);                          // 화면 전환
    ui->playGameStWidget->setCurrentWidget(playYatzee);
}

// 메뉴로 복귀
void MainWidget::returnMenu(){
    ui->playGameStWidget->setCurrentWidget(ui->page_1menu);
    ui->mainStWidget->setCurrentWidget(ui->page_1_StartMenu);   // 처음 시작 화면으로 전환

    qint32 cnt = ui->listWidgetProfile->count();

    QFile file("SaveList.txt");                                 // 프로필 화면에서 메뉴로 돌아갈 때 저장
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Open fail.";
        return;
    }

    for (int i = 0; i < cnt; i++){
        QListWidgetItem* nowProfileItem = ui->listWidgetProfile->item(i);     // 현재 선택된 프로필 가져옴
        QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfileItem);

        QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());
        QLabel* name = qobject_cast<QLabel*>(hLayout->itemAt(1)->widget());
        QLabel* maxyatzee = qobject_cast<QLabel*>(hLayout->itemAt(3)->widget());
        QLabel* max2048 = qobject_cast<QLabel*>(hLayout->itemAt(5)->widget());
        // 프로필에서 이름과 점수를 빼와서 저장

        QTextStream out(&file);                 // 이름 점수 점수 형태로 저장
        out << name->text() + " " + maxyatzee->text() + " " + max2048->text();
        if(i+1 < cnt){                          // 마지막 프로필이면 \n 제외
            out << '\n';
        }
        file.flush();
    }
    file.close();
}

void MainWidget::returnMenu2048(qint32 Max){                    // 2048에서 종료 시 호출
    QListWidgetItem* nowProfileItem = ui->listWidgetProfile->currentItem();     // 현재 선택된 프로필 가져옴
    QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfileItem);

    QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());
    QLabel* nowMax = qobject_cast<QLabel*>(hLayout->itemAt(5)->widget());       // 형변환을 통해 laber 가져옴
    qint32 nMaxInt = nowMax->text().toInt();
    if (nMaxInt < Max){                             // 2048 게임에서 받아온 max값과 프로필상 max값 비교 후
        nowMax->setText(QString::number(Max));      // 최고 기록이면 값 변경
    }

    returnMenu();                                   // 최고 기록 갱신 후 값 저장 및 화면 변경
}

void MainWidget::returnMenuyatzee(int Max){                    // yatzee에서 종료 시 호출
    ui->playGameStWidget->removeWidget(playYatzee);             // 사용이 끝난 yatzee객체 삭제
    playYatzee->deleteLater();
    playYatzee = nullptr;



    QListWidgetItem* nowProfileItem = ui->listWidgetProfile->currentItem();     // 현재 선택된 프로필 가져옴
    if (!nowProfileItem) return;         // 선택된 게 없으면 종료

    QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfileItem);
    QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());

    QLabel* nowMax = qobject_cast<QLabel*>(hLayout->itemAt(3)->widget());       // 형변환을 통해 laber 가져옴
    qint32 nMaxInt = nowMax->text().toInt();
    if (nMaxInt < Max){                             // yatzee 게임에서 받아온 max값과 프로필상 max값 비교 후
        nowMax->setText(QString::number(Max));      // 최고 기록이면 값 변경
    }

    returnMenu();                                   // 최고 기록 갱신 후 값 저장 및 화면 변경
}


//프로필 관련
void MainWidget::openProfile(){                                 // 프로필 화면으로 전환
    ui->playGameStWidget->setCurrentWidget(ui->page_2profile);
}

void MainWidget::createProfile(){                               // 프로필 생성 다이얼로그 띄움
    profileDialog createname(profileDialog::Create);
    connect(&createname,&profileDialog::sendname,[&](QString name){addlist(name);}); // 순서 중요
    createname.exec();
}

void MainWidget::addlist(QString name){                         // 다이얼로그에서 name 받아옴
    QListWidgetItem* checkitem = new QListWidgetItem(ui->listWidgetProfile);
    QWidget *containerWidget = new QWidget();                   // 여러 개의 widget합치기용
    QLabel* view1 = new QLabel("이름: ");
    QLabel* profileName = new QLabel(name);
    QLabel* view2 = new QLabel("Yacht 최고 점수: ");
    QLabel* profileYa = new QLabel("0");
    QLabel* view3 = new QLabel("2048 최고 점수: ");
    QLabel* profile2048 = new QLabel("0");
    QHBoxLayout* hLayout = new QHBoxLayout(containerWidget);    // widget 공간에 hlayout 형태로 생성
    hLayout->addWidget(view1);
    hLayout->addWidget(profileName);
    hLayout->addWidget(view2);
    hLayout->addWidget(profileYa);
    hLayout->addWidget(view3);
    hLayout->addWidget(profile2048);

    checkitem->setSizeHint(containerWidget->sizeHint());        // 시스템 자동으로 크기 배열
    ui->listWidgetProfile->setItemWidget(checkitem,containerWidget);
    ui->listWidgetProfile->setCurrentItem(checkitem);           // 프로필 생성 후 자동으로 해당 프로필 선택
}

void MainWidget::delProfile(){
    if (ui->listWidgetProfile->currentItem()){
        profileDialog delpro(profileDialog::Delete);                                  // 삭제용 다이얼로그 생성
        connect(&delpro,&profileDialog::delsig,[&](){
            QListWidgetItem* temp = ui->listWidgetProfile->currentItem();       // 현재 프로필 받아오기
            ui->listWidgetProfile->takeItem(ui->listWidgetProfile->row(temp));  // 받아온 프로필 지우기
        });
        delpro.exec();
    }
}

void MainWidget::searchProfile(){
    bool flag = 1;
    profileDialog searchpro(profileDialog::Search);
    connect(&searchpro,&profileDialog::searchsig,[&](QString name){
        for (int i = 0; i < ui->listWidgetProfile->count();i++){
            QListWidgetItem* nowProfileItem = ui->listWidgetProfile->item(i);       // 현재 선택된 프로필 가져옴
            QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfileItem);

            QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());
            QLabel* searchname = qobject_cast<QLabel*>(hLayout->itemAt(1)->widget());
            if (searchname->text() == name){                                        // 같은 이름 있을 시 멈춤
                ui->listWidgetProfile->setCurrentItem(nowProfileItem);
                flag = 0;
                break;
            }
        }
        if(flag){                                                                   // 같은 이름 없으면 없다는 다이얼로그 알림
            profileDialog noname(profileDialog::Noname);
            noname.exec();
        }

    });
    searchpro.exec();
}

void MainWidget::editProfile(){
    if (ui->listWidgetProfile->currentItem()){
        profileDialog editpro(profileDialog::Edit);
        connect(&editpro,&profileDialog::editsig,[&](QString name){

            QListWidgetItem* nowProfileItem = ui->listWidgetProfile->currentItem();    // 현재 선택된 프로필 가져옴
            QWidget* containerWidget = ui->listWidgetProfile->itemWidget(nowProfileItem);

            QHBoxLayout* hLayout = qobject_cast<QHBoxLayout*>(containerWidget->layout());
            QLabel* searchname = qobject_cast<QLabel*>(hLayout->itemAt(1)->widget());
            searchname->setText(name);
        });
        editpro.exec();
    }
}
