#ifndef YATZEE_H
#define YATZEE_H

#include <map>
#include <array>
#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>

/**
 * @brief Yatzee 게임 클래스
 * 5개의 주사위를 굴려 족보를 완성하고 점수를 획득하는 보드게임 위젯
 */
class Yatzee : public QWidget
{
    Q_OBJECT

public:
    // 생성자: 최고 점수(max)를 인자로 받아 초기화
    Yatzee(qint32 max, QWidget *parent = nullptr);
    ~Yatzee();

signals:
    // --- Signals (메인 위젯과의 통신) ---
    void gameFinished(int finalScore); // 게임 종료 시 최종 점수 전달
    void backToMenu(int score);        // 메뉴로 돌아갈 때 최고 점수 갱신값 전달

private slots:
    // --- Slots (사용자 상호작용 처리) ---
    void onRollButtonClicked();        // 'Roll Dice' 버튼 클릭 시 (주사위 굴리기)
    void onRefreshButtonClicked();     // 'Reset Game' 버튼 클릭 시 (게임 재시작)
    void onDiceChecked(int diceIndex, bool isChecked); // 주사위 버튼 클릭 시 (Keep/Unkeep 토글)
    void onBackButtonClicked();        // 'Back to Menu' 버튼 클릭 시
    void onCellClicked(int row, int column); // 점수판 셀 클릭 시 (점수 기록)
    void onRuleButtonClicked();        // 'Rules' 버튼 클릭 시 (규칙 설명 팝업)

private:
    // --- UI Setup & Resource Loading ---
    void setupUI();                    // 화면 레이아웃 및 위젯 초기화
    void loadDiceImages();             // 리소스 파일에서 주사위 이미지 로드

    // --- Game Flow Logic ---
    void resetRound();                 // 라운드 초기화 (롤 횟수, 킵 상태 리셋)
    void resetGame();                  // 게임 완전 초기화 (1라운드부터 재시작)
    void updateDiceDisplay();          // 현재 주사위 값(m_diceValues)에 맞춰 이미지 갱신
    void updateScoreTable();           // 점수판 갱신 (예상 점수 계산 및 표시)
    void updateGameInfo();             // 라운드, 남은 롤 횟수, 현재 점수 라벨 업데이트
    void endGame();                    // 게임 종료 처리 (결과창 출력)

    // --- Visual Effects (Affordance) ---
    void startRollButtonBlink();       // Roll 버튼 깜빡임 효과 시작 (사용자 유도)
    void stopRollButtonBlink();        // Roll 버튼 깜빡임 효과 중지
    void startPotentialBlink();        // 점수판의 기록 가능 항목 깜빡임 시작
    void stopPotentialBlink();         // 점수판 깜빡임 중지

    // --- Score Calculation Logic ---
    // 현재 주사위로 모든 카테고리의 예상 점수 계산
    std::map<QString, int> calculateAllScores(const std::array<int, 5>& dice);
    // 특정 카테고리의 점수 계산 (핵심 로직)
    int calculateScoreFor(const QString& category, const std::array<int, 5>& dice);

private:
    // ==========================================
    //              멤버변수
    // ==========================================

    // --- Layouts ---
    QGridLayout* m_mainLayout;         // 전체 메인 레이아웃
    QGridLayout* dice_grid_layout;     // 주사위 영역 레이아웃

    // --- UI Widgets ---
    std::array<QPushButton*, 5> m_diceButtons; // 주사위 버튼 5개 (Clickable)
    std::array<QLabel*, 5> m_keepLabels;       // 각 주사위의 'keep'라벨 5개
    QPushButton* m_rollButton;         // 주사위 굴리기 버튼
    QPushButton* m_refreshButton;      // 게임 재시작 버튼
    QPushButton* m_backButton;         // 메인 메뉴 복귀 버튼
    QPushButton* m_ruleButton;         // 게임 규칙 설명 버튼

    QTableWidget* m_scoreTable;        // 점수 기록표 (Category | Score | Potential)
    QLabel* m_roundLabel;              // 라운드 정보 표시 (Round: 1 / 13)
    QLabel* m_rollsLeftLabel;          // 남은 롤 횟수 표시 (Rolls Left: 3)
    QLabel* m_currentScoreLabel;       // 현재 총점 표시
    QLabel* m_highestScoreLabel;       // 최고 기록 표시

    // --- Game Data ---
    std::array<QPixmap, 7> m_diceImages; // 주사위 이미지 저장소 (인덱스 1~6 사용)
    std::array<int, 5> m_diceValues;     // 현재 5개 주사위의 눈금 값
    std::array<bool, 5> m_isKept;        // 각 주사위의 Keep 상태 (true: 유지, false: 굴림)

    int m_rollCount;                   // 남은 롤 횟수 (3 -> 0)
    int m_roundNumber;                 // 현재 라운드 (1 ~ 13)
    int currentScore;                  // 현재 게임 점수
    int highestScore;                  // 불러온 최고 점수

    // --- Timer & Visual State ---
    QTimer* m_blinkTimer;              // Roll 버튼 깜빡임 타이머
    QTimer* m_potentialTimer;          // 예상 점수 텍스트 깜빡임 타이머
    bool m_isBlinkStateOn;             // Roll 버튼 색상 토글 상태
    bool m_isPotentialStateOn;         // 예상 점수 텍스트 색상 토글 상태

    // --- Constants ---
    // 점수판 카테고리 이름 목록 (순서 고정)
    QStringList m_categoryNames = {
        "ACES", "TWOS", "THREES", "FOURS", "FIVES", "SIXES",
        "CHANCE", "THREE OF A KIND", "FOUR OF A KIND", "FULL HOUSE",
        "SMALL STRAIGHT", "LARGE STRAIGHT", "YATZEE"
    };

    // 카테고리별 기록된 점수 저장소 (-1: 미기록)
    std::map<QString, int> m_recordedScores;
};

#endif // YATZEE_H
