#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "game2048.h"
#include "yatzee.h"

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void returnMenu2048(qint32 Max);                // 2048 게임에서 메인 화면으로 전환하기 위한 함수, max 가져옴
    void returnMenuyatzee(int Max);              // yatzee 게임에서 메인 화면으로 전환하기 위한 함수, max 가져옴
    void addlist(QString name);                     // listwidget에 프로필을 추가하는 함수

private:
    game2048* play2048;                             // 2048 실행용 변수
    Yatzee* playYatzee;                             // yatzee 실행용 변수
    QListWidgetItem* selectProfile;                 // 프로필 선택된거 확인
    Ui::MainWidget *ui;
public slots:
    void open2048();                                // 2048 실행
    void openyatzee();                              // yatzee 실행
    void openProfile();                             // 프로필 실행
    void createProfile();                           // 프로필 생성
    void delProfile();                              // 프로필 삭제
    void returnMenu();                              // 처음 화면으로 돌아가게 하는 슬롯 함수

    void searchProfile();                           // 프로필 검색
    void editProfile();                             // 프로필 수정

};
#endif // MAINWIDGET_H
