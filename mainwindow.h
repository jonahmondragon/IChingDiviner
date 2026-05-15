#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clickablelabel.h"
#include <QLabel>
#include <QPushButton>
#include <QDockWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QPainter>
#include "iching.h"
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<QRegularExpressionMatchIterator>
#include<QVector>
#include<QPoint>
#include<QMouseEvent>
#include<QSplitter>
#include<QComboBox>
#include<QTextEdit>
#include<QIcon>
#include<QAction>
#include<QDockWidget>
#include<QDialog>
#include"aimanager.h"
#include"modelselectordialog.h"
#include<QTimer>
#include<QUrl>
#include<QSettings>
#include <QSoundEffect>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
private slots:
    void onAutoTossClicked();
    void onTossClicked();
    void onResetClicked();
    void onCoin1Clicked();
    void onCoin2Clicked();
    void onCoin3Clicked();

private:
    // UI Components
    void updateCoinPixmap(int coinIndex, bool state);
    QWidget *centralWidget;
    ClickableLabel *coinLabel1, *coinLabel2, *coinLabel3;
bool coinState0, coinState1, coinState2;
    QPushButton *autoTossButton, *tossButton, *resetButton;

    // Source hexagram components
    QLabel *sourceHexagramTitleLabel;
    QLabel *sourceHexagramDisplayLabel;  // Square area to display the hexagram
    QLabel *sourceHexagramTarotLabel;    // Rectangular area for tarot card

    // Modified hexagram components
    QLabel *modifiedHexagramTitleLabel;
    QLabel *modifiedHexagramDisplayLabel;  // Square area to display the hexagram
    QLabel *modifiedHexagramTarotLabel;    // Rectangular area for tarot card

    QDockWidget *meaningDock;
    QTextEdit *meaningTextEdit;

    // IChing instance
    IChing *iching;

    // Methods
    void setupUI();
    void setupDarkTheme();
    void updateHexagramDisplay();
    void displayCoinResults(const QList<int> &coinResults);
    void drawHexagram(QLabel *label, const QStringList &lines);

    QMap<int, QString> hexagramMeanings;
    void loadHexagramMeanings();
    void displayHexagramMeanings(int sourceHexNumber, int modifiedHexNumber);
private:
    void loadTarotCardImage(QLabel* label, int hexagramNumber);

private:
    QFrame *coinDisplayWidget;
    QIcon generateHexagramMandala(int hexagramNumber, int size);
    QPixmap paintSevenCirclesWithHex(char32_t hexagramCode, int size);
    bool isTarotMode = false;
    void createMenus();
    QAction* showCardsAction;;
    QDockWidget *controlDock;
    QTextEdit *aiTextEdit;
private:
    QComboBox *deckCombo;
    QString currentDeck;
    void populateDecksComboBox();
    QTextEdit* questionInput;

    void onSetQuestion();
    QString currentQuestion = "";
    QDialog* questionDialog = nullptr;
    void configureAIModels();
    MistralAPI* aiManager = nullptr;
    ModelSelectorDialog* modelDialog = nullptr;
    QPushButton* aiPredictionButton;
    void gatAiInterpretation();
    QString markdownToHtml(const QString &markdown);
    void toggleShuffle(bool checked);
    bool isShuffling = false;
    QTimer* mouseTimer;
    QPushButton* shuffleButton;
    QUrl datasetURL = QUrl("https://raw.githubusercontent.com/adamblvck/iching-wilhelm-dataset/master/data/iching_wilhelm_translation.js");
private:
    bool shouldFreezeUI = false;
    QSettings settings;
    QAction* downloadDatasetAction;
    void showDatasetGuide();
    QAction *aiModelsAction;

private slots:
    void loadDivination();
    void saveDivination();
    bool downloadDataset();
    void showDeckImportGuide();
    void showAIConfigGuide();

private:
    QAction* hideLabelsAction;
    QPushButton* openQuestionDialogButton;
    void resetCoins();
    QSoundEffect *coinSound;
    void playCoinSound(bool);
    bool isPlaySounds = false;
    QAction* playSoundsAction;
public:
    static bool loadChineseFont();


};

#endif // MAINWINDOW_H
