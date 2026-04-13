#include "yatzee.h"
#include "mainwidget.h"
#include <QRandomGenerator>
#include <QHeaderView>

// 생성자: 메인 위젯에서 최고 점수를 받아와 게임을 초기화합니다.
Yatzee::Yatzee(qint32 max, QWidget *parent)
    : QWidget(parent),
    m_rollCount(3),
    m_roundNumber(1),
    currentScore(0),
    highestScore(max)
{
    // 점수 기록 맵 초기화 (-1: 아직 기록되지 않음)
    for (const QString& category : m_categoryNames) {
        m_recordedScores[category] = -1;
    }

    // 주사위 킵 상태 및 값 초기화
    for(int i = 0; i < 5; i++){
        m_isKept[i] = false;
        m_diceValues[i] = 1;
    }

    loadDiceImages(); // 이미지 리소스 로드
    setupUI();        // UI 화면 구성
    resetRound();     // 첫 라운드 시작 준비

    // Roll 버튼 클릭 시 동작 연결
    connect(m_rollButton, &QPushButton::clicked, this, &Yatzee::onRollButtonClicked);
}

Yatzee::~Yatzee()
{
}

// UI 구성 및 레이아웃 설정 함수
void Yatzee::setupUI()
{
    // 1. 메인 윈도우 설정 (배경색, 크기 고정)
    this->setStyleSheet("background-color: #ECEFF1;");
    this->setFixedSize(650, 700);

    // 2. 상단 정보 라벨 생성 (라운드, 남은 롤, 현재 점수, 최고 점수)
    m_roundLabel = new QLabel(QString("Round\n%1 / 13").arg(m_roundNumber));
    QFont infoFont = m_roundLabel->font();
    infoFont.setPointSize(12);
    infoFont.setBold(true);
    m_roundLabel->setFont(infoFont);

    m_rollsLeftLabel = new QLabel(QString("Rolls Left\n%1").arg(m_rollCount));
    m_rollsLeftLabel->setFont(infoFont);

    m_currentScoreLabel = new QLabel(QString("Current Score\n%1").arg(currentScore));
    m_currentScoreLabel->setFont(infoFont);

    m_highestScoreLabel = new QLabel(QString("Highest Score\n%1").arg(highestScore));
    m_highestScoreLabel->setFont(infoFont);

    // 3. 상단 패널 (Dashboard 스타일) 구성
    QWidget* topPanel = new QWidget();
    topPanel->setFixedHeight(100);

    QHBoxLayout* topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(0, 10, 0, 10);
    topLayout->setSpacing(15);

    // 카드 형태의 스타일 시트 정의
    QString cardStyle =
        "QLabel {"
        "   background-color: white;"
        "   border-radius: 10px;"
        "   border: 1px solid #CFD8DC;"
        "   color: #37474F;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}";

    // 상단 라벨들에 스타일 적용 및 레이아웃 배치
    QList<QLabel*> infoLabels = {m_roundLabel, m_rollsLeftLabel, m_currentScoreLabel, m_highestScoreLabel};
    for(auto label : infoLabels){
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(cardStyle);
        topLayout->addWidget(label);
    }

    // Rolls Left는 중요하므로 텍스트 색상 강조
    m_rollsLeftLabel->setStyleSheet(
        cardStyle + "QLabel { color: #E64A19; }"
        );

    // 4. 하단 컨트롤 버튼 생성 및 설정
    m_ruleButton = new QPushButton("Rules");
    m_ruleButton->setMinimumHeight(40);
    m_ruleButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; border-radius: 5px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #1976D2; }"
        );
    connect(m_ruleButton, &QPushButton::clicked, this, &Yatzee::onRuleButtonClicked);

    m_rollButton = new QPushButton("Roll Dice");

    m_refreshButton = new QPushButton("Reset Game");
    m_refreshButton->setMinimumHeight(40);
    m_refreshButton->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; border-radius: 5px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #F57C00; }"
        );
    connect(m_refreshButton, &QPushButton::clicked, this, &Yatzee::onRefreshButtonClicked);

    m_backButton = new QPushButton("Back to Menu");
    m_backButton->setMinimumHeight(40);
    m_backButton->setStyleSheet(
        "QPushButton { background-color: #757575; color: white; border-radius: 5px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #616161; }"
        );
    connect(m_backButton, &QPushButton::clicked, this, &Yatzee::onBackButtonClicked);

    // 5. 타이머 설정 (시각적 효과)
    m_blinkTimer = new QTimer(this);
    m_blinkTimer->setInterval(700);
    m_isBlinkStateOn = false;

    m_potentialTimer = new QTimer(this);
    m_potentialTimer->setInterval(700);
    m_isPotentialStateOn = false;

    // Roll 버튼 깜빡임 로직
    connect(m_blinkTimer, &QTimer::timeout, this, [this](){
        m_isBlinkStateOn = !m_isBlinkStateOn;
        if(m_isBlinkStateOn){
            m_rollButton->setStyleSheet(
                "QPushButton { "
                "  background-color: #4CAF50; color: white; border-radius: 5px; font-size: 16px; font-weight: bold;"
                "  border: 4px solid #00FFFF;" // 밝은 파랑 테두리
                "}"
                );
        } else {
            m_rollButton->setStyleSheet(
                "QPushButton { "
                "  background-color: #4CAF50; color: white; border-radius: 5px; font-size: 16px; font-weight: bold;"
                "  border: 4px solid transparent;" // 기본 상태
                "}"
                );
        }
    });

    // 점수판 Potential 열 깜빡임 로직
    connect(m_potentialTimer, &QTimer::timeout, this, [this](){
        m_isPotentialStateOn = !m_isPotentialStateOn;
        for(int i = 0; i < m_categoryNames.size(); i++){
            QString category = m_categoryNames[i];
            // 이미 기록된 점수는 건드리지 않음
            if(m_recordedScores[category] != -1) {
                m_scoreTable->item(i,2)->setForeground(Qt::black);
                continue;
            }
            // 기록 가능한 칸만 빨간색/회색 점멸
            if(m_isPotentialStateOn) m_scoreTable->item(i,2)->setForeground(Qt::red);
            else m_scoreTable->item(i,2)->setForeground(QBrush(QColor("#505050")));
        }
    });

    // 6. 주사위 그리드 레이아웃 구성
    dice_grid_layout = new QGridLayout();
    QWidget* diceContainers[5]; // [라벨 + 버튼]을 묶을 컨테이너 위젯

    for (int i = 0; i < m_diceButtons.size(); i++){
        m_diceButtons[i] = new QPushButton();
        m_diceButtons[i]->setFixedSize(80,80);
        m_diceButtons[i]->setIcon(QIcon(m_diceImages[i+1]));
        m_diceButtons[i]->setIconSize(QSize(60, 60));
        m_diceButtons[i]->setCheckable(true);
        m_diceButtons[i]->setFocusPolicy(Qt::NoFocus);

        // 주사위 버튼 스타일 (체크 시 빨간 테두리)
        m_diceButtons[i]->setStyleSheet(
            "QPushButton { "
            "    border: 2px solid transparent;"
            "    border-radius: 10px;"
            "    background-color: white;"
            "}"
            "QPushButton:checked {"
            "    border: 4px solid #ff5555;"
            "    background-color: #ffeeee;"
            "}"
            );
        connect(m_diceButtons[i], &QPushButton::toggled, this, [this, i](bool checked) {
            onDiceChecked(i, checked);
        });

        // "KEEP" 라벨 생성
        m_keepLabels[i] = new QLabel("KEEP");
        m_keepLabels[i]->setAlignment(Qt::AlignCenter);
        m_keepLabels[i]->setStyleSheet("color: #ff5555; font-weight: bold; font-size: 13px;");
        m_keepLabels[i]->setVisible(false); // 기본은 숨김 상태

        // 컨테이너에 라벨과 버튼 배치
        diceContainers[i] = new QWidget();
        QVBoxLayout* vBox = new QVBoxLayout(diceContainers[i]);
        vBox->addWidget(m_keepLabels[i]);
        vBox->addWidget(m_diceButtons[i]);
    }

    // X자 형태로 주사위 배치
    dice_grid_layout->addWidget(diceContainers[0],0,0);
    dice_grid_layout->addWidget(diceContainers[1],0,2);
    dice_grid_layout->addWidget(diceContainers[2],1,1);
    dice_grid_layout->addWidget(diceContainers[3],2,0);
    dice_grid_layout->addWidget(diceContainers[4],2,2);

    // 7. 점수판 테이블 설정
    m_scoreTable = new QTableWidget(13, 3);
    QStringList headers;
    headers << "Category" << "Score" << "Potential";
    m_scoreTable->setHorizontalHeaderLabels(headers);

    m_scoreTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 세로 꽉 차게
    m_scoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 수정 불가
    m_scoreTable->verticalHeader()->setVisible(false);
    m_scoreTable->horizontalHeader()->setVisible(true);
    m_scoreTable->setSelectionMode(QAbstractItemView::NoSelection); // 선택 효과 제거
    m_scoreTable->setFocusPolicy(Qt::NoFocus);

    // 초기 아이템 생성
    for (int i = 0; i < m_categoryNames.size(); i++){
        QTableWidgetItem* itemCat = new QTableWidgetItem(m_categoryNames[i]);
        itemCat->setTextAlignment(Qt::AlignCenter);
        itemCat->setFlags(Qt::ItemIsEnabled);
        m_scoreTable->setItem(i,0,itemCat);

        QTableWidgetItem* itemScore = new QTableWidgetItem("");
        itemScore->setTextAlignment(Qt::AlignCenter);
        itemScore->setFlags(Qt::ItemIsEnabled);
        m_scoreTable->setItem(i, 1, itemScore);

        QTableWidgetItem* itemPot = new QTableWidgetItem("");
        itemPot->setTextAlignment(Qt::AlignCenter);
        itemPot->setFlags(Qt::ItemIsEnabled);
        m_scoreTable->setItem(i, 2, itemPot);
    }

    // 컬럼 너비 설정 (이름 칸은 늘어나고, 점수 칸은 고정)
    m_scoreTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_scoreTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    m_scoreTable->setColumnWidth(1, 70);
    m_scoreTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_scoreTable->setColumnWidth(2,70);

    // 테이블 스타일 시트 설정
    m_scoreTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: #ffffff;"
        "   alternate-background-color: #f2f2f2;"
        "   gridline-color: #d0d0d0;"
        "   border: 2px solid #333333;"
        "   font-size: 14px;"
        "   selection-background-color: transparent;"
        "   selection-color: black;"
        "}"
        "QHeaderView::section {"
        "   background-color: #404040;"
        "   color: white;"
        "   padding: 4px;"
        "   border: 1px solid #6c6c6c;"
        "   font-weight: bold;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: transparent;"
        "   color: black;"
        "}"
        "QTableWidget::item:focus {"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        );
    m_scoreTable->setAlternatingRowColors(true);
    connect(m_scoreTable, &QTableWidget::cellClicked, this, &Yatzee::onCellClicked);

    // 8. 메인 레이아웃 배치
    m_mainLayout = new QGridLayout(this);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    m_mainLayout->setSpacing(15);

    m_mainLayout->addWidget(topPanel,0,0,1,3); // 상단 패널

    m_mainLayout->addWidget(m_scoreTable, 1, 0, 4, 2); // 왼쪽 점수판

    m_mainLayout->addWidget(m_rollButton, 1, 2); // 오른쪽 롤 버튼
    m_mainLayout->addLayout(dice_grid_layout, 2, 2, 3, 1); // 오른쪽 주사위

    // 하단 버튼 배치
    m_mainLayout->addWidget(m_backButton, 5, 0);
    m_mainLayout->addWidget(m_ruleButton,5,1);
    m_mainLayout->addWidget(m_refreshButton, 5, 2);

    m_mainLayout->setColumnStretch(0, 1);
    m_mainLayout->setColumnStretch(1, 1);
    m_mainLayout->setColumnStretch(2, 1);
}

// 주사위 이미지 리소스 로드
void Yatzee::loadDiceImages()
{
    m_diceImages[1] = QPixmap(":/yatzee/resourceyatzee/dice-six-faces-one.png");
    m_diceImages[2] = QPixmap(":/yatzee/resourceyatzee/dice-six-faces-two.png");
    m_diceImages[3] = QPixmap(":/yatzee/resourceyatzee/dice-six-faces-three.png");
    m_diceImages[4] = QPixmap(":/yatzee/resourceyatzee/dice-six-faces-four.png");
    m_diceImages[5] = QPixmap(":/yatzee/resourceyatzee/dice-six-faces-five.png");
    m_diceImages[6] = QPixmap(":/yatzee/resourceyatzee/dice-six-faces-six.png");
}

// 라운드 시작 시 상태 초기화
void Yatzee::resetRound()
{
    stopPotentialBlink(); // 깜빡임 중지

    m_rollCount = 3; // 롤 횟수 리셋
    for(int i=0; i<5; ++i) m_isKept[i] = false;

    // 버튼 상태 초기화 (체크 해제, 비활성화)
    for(int i=0; i<5; ++i){
        if(m_diceButtons[i]){
            m_diceButtons[i]->setChecked(false);
            m_diceButtons[i]->setCheckable(false); // 굴리기 전엔 Keep 불가
        }
        if(m_keepLabels[i]) {
            m_keepLabels[i]->setVisible(false);
        }
    }
    startRollButtonBlink(); // 롤 버튼 깜빡임 시작

    updateGameInfo();
    updateScoreTable();
}

// 현재 주사위 값에 따라 버튼 이미지 업데이트
void Yatzee::updateDiceDisplay()
{
    for(int i = 0; i < 5; i++){
        int val = m_diceValues[i];
        if(val >= 1 && val <= 6)
            m_diceButtons[i]->setIcon(QIcon(m_diceImages[val]));
    }
}

// 점수판 갱신 (기록된 점수 및 예상 점수 표시)
void Yatzee::updateScoreTable()
{
    for(int i = 0; i < m_categoryNames.size(); i++) {
        QString category = m_categoryNames[i];
        int recordedScore = m_recordedScores[category];

        QTableWidgetItem* scoreItem = m_scoreTable->item(i,1);
        QTableWidgetItem* potentialItem = m_scoreTable->item(i,2);

        if(recordedScore != -1) {
            // 이미 기록된 경우 점수 표시
            scoreItem->setText(QString::number(recordedScore));
            potentialItem->setText("-");
            potentialItem->setForeground(Qt::black);
        }
        else {
            // 기록되지 않은 경우
            scoreItem->setText("");
            if(m_rollCount == 3) {
                // 아직 주사위를 굴리지 않았으면 비움
                potentialItem->setText("");
                potentialItem->setForeground(Qt::black);
            } else {
                // 주사위를 굴렸다면 예상 점수 표시
                int potential = calculateScoreFor(category, m_diceValues);
                potentialItem->setText(QString::number(potential));
            }
        }
    }
}

// 게임 정보(라운드, 남은 롤, 점수) 라벨 업데이트
void Yatzee::updateGameInfo()
{
    m_roundLabel->setText(QString("Round\n%1 / 13").arg(m_roundNumber));
    m_rollsLeftLabel->setText(QString("Rolls Left\n%1").arg(m_rollCount));

    // 현재 총점 계산
    currentScore = 0;
    for(auto const& [key, val] : m_recordedScores) {
        if(val != -1) currentScore += val;
    }
    m_currentScoreLabel->setText(QString("Current Score\n%1").arg(currentScore));
    m_highestScoreLabel->setText(QString("Highest Score\n%1").arg(highestScore));
}

// 모든 카테고리의 예상 점수 계산
std::map<QString, int> Yatzee::calculateAllScores(const std::array<int, 5>& dice)
{
    std::map<QString, int> allScores;
    for(const QString& category : m_categoryNames){
        allScores[category] = calculateScoreFor(category, dice);
    }
    return allScores;
}

// 특정 카테고리의 점수 계산 로직
int Yatzee::calculateScoreFor(const QString& category, const std::array<int, 5>& dice)
{
    std::array<int,7> counts; // 주사위 눈 개수 카운팅 (1~6)
    counts.fill(0);
    for(int i = 0; i < 5; i++){
        counts[dice[i]] += 1;
    }

    int sum = 0;
    int totalDiceSum = 0; // 주사위 5개 총합
    for(int d : dice) totalDiceSum += d;

    // --- Upper Section (1~6 눈금 합계) ---
    if(category == "ACES") {
        for(auto a : dice) if(a==1) sum+= a;
    }
    else if(category == "TWOS") {
        for(auto a: dice) if(a==2) sum+= a;
    }
    else if(category == "THREES") {
        for(auto a: dice) if(a==3) sum+= a;
    }
    else if(category == "FOURS") {
        for(auto a: dice) if(a==4) sum+= a;
    }
    else if(category == "FIVES") {
        for(auto a: dice) if(a==5) sum+= a;
    }
    else if(category == "SIXES") {
        for(auto a: dice) if(a==6) sum+= a;
    }

    // --- Lower Section (족보 조합) ---
    else if(category == "CHANCE") {
        sum = totalDiceSum;
    }
    else if(category == "THREE OF A KIND") {
        for(int i = 1; i <= 6; i++){
            if(counts[i] >= 3){
                sum = totalDiceSum;
                break;
            }
        }
    }
    else if(category == "FOUR OF A KIND") {
        for(int i = 1; i <= 6; i++){
            if(counts[i] >= 4){
                sum = totalDiceSum;
                break;
            }
        }
    }
    else if(category == "FULL HOUSE") {
        bool two_found = false;
        bool three_found = false;
        for(int i = 1; i <= 6; i++){
            if(counts[i] == 3) three_found = true;
            if(counts[i] == 2) two_found = true;
        }
        if(two_found && three_found) sum = 25;
    }
    else if(category == "SMALL STRAIGHT") {
        // 연속된 숫자 4개 확인 (1-2-3-4, 2-3-4-5, 3-4-5-6)
        bool p1 = (counts[1] && counts[2] && counts[3] && counts[4]);
        bool p2 = (counts[2] && counts[3] && counts[4] && counts[5]);
        bool p3 = (counts[3] && counts[4] && counts[5] && counts[6]);
        if(p1 || p2 || p3) sum = 30;
    }
    else if(category == "LARGE STRAIGHT") {
        // 연속된 숫자 5개 확인 (1-2-3-4-5, 2-3-4-5-6)
        bool p1 = (counts[1] && counts[2] && counts[3] && counts[4] && counts[5]);
        bool p2 = (counts[2] && counts[3] && counts[4] && counts[5] && counts[6]);
        if(p1 || p2) sum = 40;
    }
    else if(category == "YATZEE") {
        for(int i = 1; i <= 6; i++){
            if(counts[i] == 5){
                sum = 50;
                break;
            }
        }
    }
    return sum;
}

// 점수판 셀 클릭 시 점수 기록 처리
void Yatzee::onCellClicked(int row, int column)
{
    if (m_rollCount == 3) return; // 아직 굴리지 않았으면 무시
    if (column != 2) return;      // Potential 컬럼만 클릭 가능

    QString category = m_categoryNames[row];

    if (m_recordedScores[category] != -1) {
        return; // 이미 기록된 칸 무시
    }

    int score = calculateScoreFor(category, m_diceValues);

    // 0점 기록 시 경고창 표시 (실수 방지)
    if (score == 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Zero Score Warning",
                                      QString("You are about to record 0 points for '%1'.\nAre you sure?").arg(category),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }

    // 점수 확정 및 기록
    m_recordedScores[category] = score;

    // 다음 라운드 진행 또는 게임 종료
    if (m_roundNumber < 13) {
        m_roundNumber++;
        resetRound();
    } else {
        updateGameInfo();
        updateScoreTable();
        endGame();
    }
}

// Roll Dice 버튼 클릭 슬롯
void Yatzee::onRollButtonClicked()
{
    stopRollButtonBlink(); // 깜빡임 중지

    if(m_rollCount > 0){
        for(int i = 0; i < 5; i++){
            // Keep 안 된 주사위만 무작위로 굴림
            if(!m_isKept[i]){
                int randomValue = QRandomGenerator::global()->bounded(1,7);
                m_diceValues[i] = randomValue;
                m_diceButtons[i]->setIcon(QIcon(m_diceImages[randomValue]));
            }
            m_diceButtons[i]->setCheckable(true); // 굴린 후에는 Keep 가능
        }
        m_rollCount--;

        updateGameInfo();
        updateScoreTable();
        startPotentialBlink(); // 점수 기록 유도 깜빡임 시작
    }
}

// 주사위 버튼 클릭(Keep/Unkeep) 처리
void Yatzee::onDiceChecked(int diceIndex, bool isChecked)
{
    m_keepLabels[diceIndex]->setVisible(isChecked); // KEEP 라벨 표시/숨김
    m_isKept[diceIndex] = isChecked;

    // 모든 주사위가 Keep 상태인지 확인
    bool isAllKept = true;
    for(int i = 0; i < 5; i++){
        if(!m_isKept[i]){
            isAllKept = false;
            break;
        }
    }
    // 롤 횟수가 남았어도 다 Keep하면 굴리기 버튼 비활성화
    if(m_rollCount > 0){
        if (isAllKept) m_rollButton->setDisabled(true);
        else m_rollButton->setDisabled(false);
    }
}

// --- 타이머 관련 함수들 ---
void Yatzee::startRollButtonBlink() {
    if (!m_blinkTimer->isActive()) m_blinkTimer->start();
}

void Yatzee::stopRollButtonBlink() {
    if (m_blinkTimer->isActive()) m_blinkTimer->stop();
    // 멈출 때는 기본 스타일로 복구
    m_rollButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #4CAF50; color: white; border-radius: 5px; font-size: 16px; font-weight: bold;"
        "  border: 2px solid transparent;"
        "}"
        "QPushButton:disabled { background-color: #cccccc; color: #666666; }"
        "QPushButton:hover { background-color: #45a049; }"
        );
}

void Yatzee::startPotentialBlink() {
    if (!m_potentialTimer->isActive()) m_potentialTimer->start();
}

void Yatzee::stopPotentialBlink() {
    if (m_potentialTimer->isActive()) m_potentialTimer->stop();
}

// --- 네비게이션 버튼 슬롯 ---
void Yatzee::onRefreshButtonClicked() {
    resetGame();
}

void Yatzee::onBackButtonClicked() {
    resetGame();
    emit backToMenu(highestScore);
}

// 게임 종료 처리 및 결과창 표시
void Yatzee::endGame()
{
    stopPotentialBlink();
    stopRollButtonBlink();

    m_rollButton->setEnabled(false);
    m_rollButton->setText("Game Over");

    // 최고 점수 갱신 및 저장
    if(currentScore > highestScore) {
        highestScore = currentScore;
        m_highestScoreLabel->setText(QString("Highest Score\n%1").arg(highestScore));
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(QString("Game Finished!\nYour Total Score: %1").arg(currentScore));
    msgBox.setIcon(QMessageBox::Information);

    QAbstractButton* retryBtn = msgBox.addButton(QMessageBox::Retry);
    QAbstractButton* quitBtn = msgBox.addButton("Quit Game", QMessageBox::RejectRole);

    msgBox.exec(); // 모달 실행

    if(msgBox.clickedButton() == retryBtn){
        resetGame();
    }
    else{
        emit backToMenu(highestScore);
    }
    emit gameFinished(currentScore);
}

// 게임 완전 초기화 (1라운드로 복귀)
void Yatzee::resetGame()
{
    m_roundNumber = 1;
    currentScore = 0;

    // 점수 데이터 초기화
    for (const QString& category : m_categoryNames) {
        m_recordedScores[category] = -1;
    }

    // 테이블 텍스트 초기화
    for(int i = 0; i < m_categoryNames.size(); i++) {
        m_scoreTable->item(i, 1)->setText("");
        m_scoreTable->item(i, 2)->setText("");
        m_scoreTable->item(i, 2)->setForeground(Qt::black);
    }

    m_rollButton->setEnabled(true);
    m_rollButton->setText("Roll Dice");
    m_currentScoreLabel->setText("Current Score: 0");

    resetRound();
}

// 룰 설명 팝업 표시
void Yatzee::onRuleButtonClicked()
{
    QString rules =
        "<h2>Yahtzee Rules</h2>"
        "<p><b>Objective:</b> Score points by rolling five dice to make certain combinations.</p>"
        "<p><b>Gameplay:</b></p>"
        "<ul>"
        "<li>The game consists of <b>13 rounds</b>.</li>"
        "<li>In each round, you can roll the dice up to <b>3 times</b>.</li>"
        "<li>Click dice to <b>Keep</b> them before re-rolling.</li>"
        "<li>First Throw is compulsory. You can't choose a category before re-rolling.</li>"
        "<li>After rolling, click a category in the score table to record your score.</li>"
        "</ul>"

        "<hr>"

        "<p><b>1. Upper Section Scoring:</b></p>"
        "<p>Sum of specific dice numbers (Aces~Sixes).</p>"
        "<img src=':/yatzee/resourceyatzee/rule1.png' width='500'>"

        "<br>"

        "<p><b>2. Lower Section Scoring:</b></p>"
        "<p>Poker-style combinations (Full House, Straights, Yahtzee).</p>"
        "<img src=':/yatzee/resourceyatzee/rule2.png' width='500'>"

        "<br><br>"
        "<p><i>Good Luck!</i></p>";

    QMessageBox::about(this, "Game Rules", rules);
}
