#ifndef GAME2_48_H
#define GAME2_48_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator64>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QTimer>
#include <QParallelAnimationGroup>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class game2048;
}
QT_END_NAMESPACE

class game2048 : public QWidget
{
    Q_OBJECT
public:
    explicit game2048(qint32 Max, QWidget *parent = nullptr);
    void rand_num();                // 랜덤 넘버 생성

    void board_clear();             // 보드를 지우는 함수
    void view();                    // 보드 표시

    bool check_game_over();         // 게임 오버 여부 확인
    bool check_game_clear();        // 게임 성공 여부 확인

    void left();                    // 화살표 누를 시 동작
    void right();
    void up();
    void down();

    ~game2048();

public slots:
    void undo();                    // undo 버튼 누를 시 동작
    void newGame();                 // new 버튼 누를 시 동작
    void returnMenu();              // return 메뉴 누를 시 동작

private:
    QGridLayout *gridLayout;        // 게임 보드 추가

    qint32 undocount;               // undo 남은 횟수, undo 가능 여부
    bool undocheck;

    qint32 score, maxscore;         // 점수 ,최대 점수

    qint32 board[4][4] = {0};       // 보드에 숫자 저장용 배열
    qint32 undoboard[4][4] = {0};       // 보드에 숫자 저장용 배열

    QPixmap num_img[11];            // 아이콘 및 라벨
    QLabel* boardview[4][4];        // 보드에 저장된 숫자 출력용 배열, 0이면 안보이게하고 나머지는 보이게 할 예정

    void keyPressEvent(QKeyEvent *event) override;       // 이렇게 오버라이드 해줘야 동작한다고 함
    qint32 act;                     // 실제로 움직였는지 확인용

    Ui::game2048 *ui;

};

#endif // GAME2_48_H
