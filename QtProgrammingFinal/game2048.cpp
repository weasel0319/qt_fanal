#include "./ui_game2048.h"
#include "game2048dialog.h"
#include "mainwidget.h"
#include "game2048.h"

game2048::game2048(qint32 Max, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::game2048)
{
    setFocusPolicy(Qt::StrongFocus);  // 포커스를 해야 버튼에 방향키 오버라이드 된 걸 무시하고
    setFocus();                       // 내 오버라이드 함수가 동작한다고 함
    ui->setupUi(this);
    num_img[0].load(":/resource2048/2.png");
    num_img[1].load(":/resource2048/4.png");
    num_img[2].load(":/resource2048/8.png");
    num_img[3].load(":/resource2048/16.png");
    num_img[4].load(":/resource2048/32.png");
    num_img[5].load(":/resource2048/64.png");
    num_img[6].load(":/resource2048/128.png");
    num_img[7].load(":/resource2048/256.png");
    num_img[8].load(":/resource2048/512.png");
    num_img[9].load(":/resource2048/1024.png");
    num_img[10].load(":/resource2048/2048.png"); // 보드의 사진 로드

    undocount = 2; // undo 버튼 초기 사용 횟수 설정 및 버튼 설정
    undocheck = 0;
    ui->undoBt->setIcon(QIcon(":/resource2048/undo.png"));
    ui->undoBt->setText("Undo");

    gridLayout = new QGridLayout();                     // board 생성
    gridLayout->setSpacing(5);                          // 타일 간의 간격 설정
    gridLayout->setContentsMargins(10, 10, 10, 10);     // 컨테이너 패딩
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            boardview[i][j] = new QLabel("");
            // Horizontal/Vertical 모두 Expanding으로 설정
            boardview[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(boardview[i][j], i, j);

            boardview[i][j]->setScaledContents(true);
            gridLayout->setColumnStretch(j, 1);
            gridLayout->setRowStretch(i, 1);
        }
    }

    ui->mainboardLayout->addLayout(gridLayout);     // grid레이아웃 추가
    rand_num(); rand_num();                     // 초기값 생성
    view();                                     // 화면 세팅

    score = 0;
    ui->scoreLabel->setText("Score\n" + QString::number(score));
    maxscore = Max;
    ui->maxLabel->setText("Best\n" + QString::number(maxscore));    // 점수 표시

    ui->widget->setStyleSheet("background-color: rgb(255, 170, 110);");
    ui->scoreLabel->setStyleSheet(                                  // 디자인
        "QLabel#scoreLabel {"
        "background-color: rgb(244, 115, 31);"
        "color: white;"
        "font-size: 16pt;"
        "font-weight: bold;"
        "padding: 5px 10px;"
        "border: 2px solid rgb(200, 95, 20);"
        "border-radius: 5px;"
        "}"
        );

    ui->maxLabel->setStyleSheet(
        "QLabel#maxLabel {"
        "background-color: rgb(220, 95, 20);"
        "color: white;"
        "font-size: 16pt;"
        "font-weight: bold;"
        "padding: 5px 10px;"
        "border: 2px solid rgb(200, 95, 20);"
        "border-radius: 5px;"
        "}"
        );

    ui->undoBt->setStyleSheet(
        "QPushButton#undoBt {"
        "background-color: rgb(0, 128, 0);"
        "color: white;"
        "font-size: 14pt;"
        "font-weight: bold;"
        "padding: 5px 10px;"
        "border: 2px outset rgb(0, 80, 0);"
        "border-radius: 5px;"
        "}"
        );

    ui->undoText->setStyleSheet(
        "QLabel#undoText {"
        "background-color: transparent;"
        "color: rgb(60, 60, 60);"
        "font-size: 10pt;"
        "font-weight: bold;"
        "padding: 5px;"
        "}"
        );
    ui->newBtn->setStyleSheet(
        "QPushButton#newBtn {"
        "background-color: rgb(255, 60, 60);"
        "color: white;"
        "font-size: 10pt;"
        "font-weight: bold;"
        "padding: 5px 10px;"
        "border: 2px outset rgb(200, 40, 40);"
        "border-radius: 5px;"
        "}"
        );
    ui->returnBtn->setStyleSheet(
        "QPushButton#returnBtn {"
        "background-color: rgb(70, 140, 200);"
        "color: white;"
        "font-size: 10pt;"
        "font-weight: bold;"
        "padding: 5px 10px;"
        "border: 2px outset rgb(50, 100, 160);"
        "border-radius: 5px;"
        "}"
        );
    setLayout(ui->boardLayout);                 // 현재 화면 적용
}

// 키보드 입력 시 동작 설정
void game2048::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right || event->key() == Qt::Key_Up || event->key() == Qt::Key_Down){ // 다른 키 입력 무시
        switch(event->key()){
        case Qt::Key_Left:
            left();
            break;
        case Qt::Key_Right:
            right();
            break;
        case Qt::Key_Up:
            up();
            break;
        case Qt::Key_Down:
            down();
            break;
        }

        if (act > 0){
            for (int i = 0; i < 4; i++){                // undo 기능 구현을 위한 이전 배열 저장
                for (int j = 0; j < 4; j++){
                    undoboard[i][j] = board[i][j];
                }
            }
            undocheck = 1;                              // 1회 움직였을 시 undo 기능 활성화

            if (score > maxscore){                      // 최대 점수 넘어가면 max 점수 표기
                maxscore = score;
                ui->maxLabel->setText("Best\n" + QString::number(maxscore));
            }
            rand_num();                                 // 랜덤 값 생성
            QTimer::singleShot(100,[=](){ view();});    // 애니메이션을 기다리기 위해 100ms 후 화면 결과 표시
            board_clear();                              // 다음 키 입력을 위한 board 준비

            if (check_game_clear()){                    // 게임 클리어시 다이얼로그 생성
                game2048dialog over(score, maxscore);
                connect(&over,&game2048dialog::newGame,this,&game2048::newGame);
                connect(&over,&game2048dialog::returnMenu,this,&game2048::returnMenu);
                over.exec();
            }

            if (check_game_over()){                     // 게임 오버시 다이얼로그 생성, undo 횟수가 남아있을 때 사용 가능
                if (undocount == 0){
                    game2048dialog over2(game2048dialog::Noundo);
                    connect(&over2,&game2048dialog::newGame,this,&game2048::newGame);
                    connect(&over2,&game2048dialog::returnMenu,this,&game2048::returnMenu);
                    over2.exec();

                }
                else{
                    game2048dialog over(game2048dialog::Over);
                    connect(&over,&game2048dialog::newGame,this,&game2048::newGame);
                    connect(&over,&game2048dialog::returnMenu,this,&game2048::returnMenu);
                    connect(&over,&game2048dialog::undoGame,this,&game2048::undo);
                    over.exec();
                }
            }
        }
    }
}




void game2048::newGame(){
    undocount = 2; // undo 버튼 사용 횟수 설정, 사용 가능하게 만듬
    undocheck = 0;
    ui->undoText->setText("2 use left");    // 남은 횟수 표시
    score = 0;
    ui->scoreLabel->setText("Score\n" + QString::number(score));    // 점수 초기화 표기


    for (int i = 0; i < 4; i++){            // 보드 초기화
        for(int j = 0; j < 4; j++){
            board[i][j] = 0;
        }
    }
    rand_num(); rand_num();                     // 초기값 생성
    view();                                     // 보드 표시
}

void game2048::returnMenu(){
    QWidget *parentWidget = nativeParentWidget();                       // 가장 가까운 부모 위젯 가져오기(mainwidget) 그냥 ParentWidget은 오류
    MainWidget *stwidget = qobject_cast<MainWidget*>(parentWidget);
    stwidget->returnMenu2048(maxscore);         // 현재 최댓값 전달
    this->close();
}

bool game2048::check_game_over(){
    int i, j;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] == 0){
                return 0; // 빈 칸이 있으면 게임 오버 아님
            }
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            int current_val = board[i][j];

            if (j < 3 && current_val == board[i][j+1]){
                return 0; // 합칠 수 있는 타일이 있어 게임 오버 아님
            }

            if (i < 3 && current_val == board[i+1][j]){
                return 0; // 합칠 수 있는 타일이 있어 게임 오버 아님
            }
        }
    }
    return 1;
}

bool game2048::check_game_clear(){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] == 2048){
                return 1;                       // 보드에 2048이 있으면 클리어
            }
        }
    }
    return 0;
}





void game2048::view(){                          // 화면에 값에 맞는 사진 표시
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            switch(board[i][j]){
            case 0:
                boardview[i][j]->clear();
                break;
            case 2:
                boardview[i][j]->setPixmap(num_img[0].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 4:
                boardview[i][j]->setPixmap(num_img[1].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 8:
                boardview[i][j]->setPixmap(num_img[2].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 16:
                boardview[i][j]->setPixmap(num_img[3].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 32:
                boardview[i][j]->setPixmap(num_img[4].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 64:
                boardview[i][j]->setPixmap(num_img[5].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 128:
                boardview[i][j]->setPixmap(num_img[6].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 256:
                boardview[i][j]->setPixmap(num_img[7].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 512:
                boardview[i][j]->setPixmap(num_img[8].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 1024:
                boardview[i][j]->setPixmap(num_img[9].scaled(100,100, Qt::KeepAspectRatio));
                break;
            case 2048:
                boardview[i][j]->setPixmap(num_img[10].scaled(100,100, Qt::KeepAspectRatio));
                break;
            }
        }
    }
}





// 키보드 이동 후 중복 방지를 위해 더한 값을 빼기
void game2048::board_clear(){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] > 10000) board[i][j] -= 10000;
        }
    }
}

//랜덤 값 배열에 생성
void game2048::rand_num(){                          // 비어있는 배열을 찾은 후 그 배열의 주소를 담아 랜덤한 값 대입
    qint32* generator[16] = {0};
    int cnt = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] == 0){
                generator[cnt++] = &board[i][j];
            }
        }
    }
    *generator[QRandomGenerator::global()->bounded(cnt)] = (QRandomGenerator::global()->bounded(10) < 7) ? 2 : 4;
}

// undo 버튼 누를 시 이전으로 복구 (2회만 가능)
void game2048::undo(){
    if (undocount > 0){
        if (undocheck){                             // 직전에 undo 사용했으면 사용 불가
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                    board[i][j] = undoboard[i][j];
                }
            }
            undocheck = 0;                          // 사용했다는 알림
            ui->undoText->setText(QString::number(--undocount) + " use left");    // 남은 횟수 표시
            view();
        }
        else{                                       // 두번 연속 불가능
            ui->undoText->setText(QString::number(undocount) + " use left, move first");
        }
    }
}









// 키보드 동작
void game2048::left(){                  // 왼쪽 키보드 누를 시 동작
    act = 0;
    for (int i = 0; i < 4; i++){        // 왼쪽에서 시작
        for (int j = 1; j <= 3; j++){   // 1부터 시작해 왼쪽값이랑 비교
            for (int k = j; k > 0; k--){    // 왼쪽
                if (board[i][k] == 0 || board[i][k] > 10000) break;    // 값이 이미 계산된 결과거나 0일 때
                if (board[i][k-1] != 0 && board[i][k-1] != board[i][k]) break; // 왼쪽에 값이 있고, 현재값과 다를 때
                if (board[i][k-1] == 0) board[i][k-1] = board[i][k];    // 왼쪽값이 없으면 한칸 밈
                else if (board[i][k] == board[i][k-1]){                 // 값이 같으면
                    board[i][k-1] *= 2;                                 // 값을 곱해주고(더하기)
                    board[i][k-1] += 10000;                             // 이미 계산했다고 표시

                    score += (board[i][k] * 2);
                    ui->scoreLabel->setText("Score\n" + QString::number(score));    // 점수 표기
                }
                QPropertyAnimation *anim = new QPropertyAnimation(boardview[i][k], "pos", this);
                QPoint positionFrom = boardview[i][k]->pos();           // 이동했을 때 현재 위치
                QPoint positionTo = boardview[i][k-1]->pos();           // 이동했을 때 도착할 위치
                anim->setStartValue(positionFrom);
                anim->setEndValue(positionTo);
                anim->setDuration(50);
                anim->start();                                          // 애니메이션 이동시작

                board[i][k] = 0;                                        // 현재 값 초기화, 동작 알림
                act++;
            }
        }
    }
}

void game2048::right(){                 // 오른쪽도 왼쪽과 같은 알고리즘
    act = 0;
    for (int i = 3; i >= 0; i--){
        for (int j = 2; j >= 0; j--){   // 시작 위치만 반대로 바꿔줌
            for (int k = j; k < 3; k++){
                if (board[i][k] == 0 || board[i][k] > 10000) break;
                if (board[i][k+1] != 0 && board[i][k+1] != board[i][k]) break; // k-1 이 아닌 +1로 오른쪽 값에 접근
                if (board[i][k+1] == 0) board[i][k+1] = board[i][k];
                else if (board[i][k] == board[i][k+1]){
                    board[i][k+1] *= 2;
                    board[i][k+1] += 10000;

                    score += (board[i][k] * 2);
                    ui->scoreLabel->setText("Score\n" + QString::number(score));    // 점수 표기
                }
                QPropertyAnimation *anim = new QPropertyAnimation(boardview[i][k], "pos", this);
                QPoint positionFrom = boardview[i][k]->pos();
                QPoint positionTo = boardview[i][k+1]->pos();
                anim->setStartValue(positionFrom);
                anim->setEndValue(positionTo);
                anim->setDuration(50);
                anim->start();

                board[i][k] = 0;
                act++;
            }
        }
    }
}

void game2048::up(){                    // 위와 같은 알고리즘
    act = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 1; j <= 3; j++){
            for (int k = j; k > 0; k--){
                if (board[k][i] == 0 || board[k][i] > 10000) break;     // board 배열의 열을 먼저 접근
                if (board[k-1][i] != 0 && board[k-1][i] != board[k][i]) break;
                if (board[k-1][i] == 0) board[k-1][i] = board[k][i];
                else if (board[k][i] == board[k-1][i]){
                    board[k-1][i] *= 2;
                    board[k-1][i] += 10000;

                    score += (board[k][i] * 2);
                    ui->scoreLabel->setText("Score\n" + QString::number(score));    // 점수 표기
                }
                QPropertyAnimation *anim = new QPropertyAnimation(boardview[k][i], "pos", this);
                QPoint positionFrom = boardview[k][i]->pos();
                QPoint positionTo = boardview[k-1][i]->pos();
                anim->setStartValue(positionFrom);
                anim->setEndValue(positionTo);
                anim->setDuration(50);
                anim->start();

                board[k][i] = 0;
                act++;
            }
        }
    }
}

void game2048::down(){              // up과 같지만 right 처럼 +1로 아래의 값을 먼저 접근
    act = 0;
    for (int i = 3; i >= 0; i--){
        for (int j = 2; j >= 0; j--){
            for (int k = j; k < 3; k++){
                if (board[k][i] == 0 || board[k][i] > 10000) break;
                if (board[k+1][i] != 0 && board[k+1][i] != board[k][i]) break;
                if (board[k+1][i] == 0) board[k+1][i] = board[k][i];
                else if (board[k][i] == board[k+1][i]){
                    board[k+1][i] *= 2;
                    board[k+1][i] += 10000;

                    score += (board[k][i] * 2);
                    ui->scoreLabel->setText("Score\n" + QString::number(score));    // 점수 표기
                }
                QPropertyAnimation *anim = new QPropertyAnimation(boardview[k][i], "pos", this);
                QPoint positionFrom = boardview[k][i]->pos();
                QPoint positionTo = boardview[k+1][i]->pos();
                anim->setStartValue(positionFrom);
                anim->setEndValue(positionTo);
                anim->setDuration(50);
                anim->start();

                board[k][i] = 0;
                act++;
            }
        }
    }
}

game2048::~game2048()
{
    delete ui;
}
