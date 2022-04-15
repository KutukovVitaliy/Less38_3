#include <QtGui/QtGui>
#include <QApplication>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>
#include <QSlider>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget playerWindow;
    QVBoxLayout vbox(&playerWindow);
    QHBoxLayout buttons;
    auto *videoWidget = new QVideoWidget(&playerWindow);
    QSlider *songSlider = new QSlider (&playerWindow);
    songSlider->setOrientation(Qt::Horizontal);
    vbox.addWidget(videoWidget);
    vbox.addWidget(songSlider);

    auto *openFileButton = new QPushButton("Open", &playerWindow);
    auto *playMediaButton = new QPushButton("Play", &playerWindow);
    auto *pauseMediaButton = new QPushButton("Pause", &playerWindow);
    auto *stopMediaButton = new QPushButton("Stop", &playerWindow);

    buttons.addWidget(openFileButton);
    buttons.addWidget(playMediaButton);
    buttons.addWidget(pauseMediaButton);
    buttons.addWidget(stopMediaButton);

    vbox.addLayout(&buttons);

    QString filePath;
    auto *audioOutput = new QAudioOutput;
    auto *player = new QMediaPlayer(&playerWindow);
    player->setAudioOutput(audioOutput);
    player->setVideoOutput(videoWidget);
    audioOutput->setVolume(50);
    bool isPlaying = false;
    qint64 currentPosition = 0;

    QObject::connect(openFileButton, &QPushButton::clicked, [&filePath, player, &isPlaying, &playerWindow, songSlider, &currentPosition]()
    {
       filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "C:\\Qt\\Projects\\Less38_3", "media files(*.mp3, *.mp4)");
       player->stop();
       isPlaying = false;
       playerWindow.setWindowTitle(filePath);
       player->setSource(QUrl::fromLocalFile(filePath));
       QObject::connect(player, &QMediaPlayer::durationChanged, [songSlider](qint64 duration)
       {
           songSlider->setMinimum(0);
           songSlider->setMaximum(duration);
       });
       QObject::connect(player, &QMediaPlayer::positionChanged, [songSlider, &currentPosition](qint64 pos)
       {
           songSlider->setValue(pos);
           currentPosition = pos;
       });
    });
    QObject::connect(playMediaButton, &QPushButton::clicked, [player, &currentPosition, &isPlaying]()
    {
        isPlaying = true;
        player->play();
        player->setPosition(currentPosition);
    });
    QObject::connect(songSlider, &QSlider::valueChanged, [player](int pos)
    {
        player->setPosition(pos);
    });
    QObject::connect(stopMediaButton, &QPushButton::clicked, [player, &currentPosition, songSlider, &isPlaying](){
        player->stop();
        isPlaying = false;
        songSlider->setValue(0);
        currentPosition = 0;
    });
    QObject::connect(pauseMediaButton, &QPushButton::clicked, [player, &isPlaying](){

        player->pause();
        isPlaying = false;

    });
    playerWindow.setWindowTitle("Multimedia player");
    playerWindow.show();
    return a.exec();
}
