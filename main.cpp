#include <QtGui/QtGui>
#include <QApplication>

#include <QMediaPlayer>
#include <QAudioOutput>
//#include <QMediaContent>

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
    QSlider *songSlider = new QSlider (&playerWindow);
    songSlider->setOrientation(Qt::Horizontal);
    vbox.addWidget(songSlider);

    auto *openFileButton = new QPushButton("Open", &playerWindow);
    auto *playMediaButton = new QPushButton("Play", &playerWindow);
    auto *pauseMeiaButton = new QPushButton("Pause", &playerWindow);
    auto *stopMediaButton = new QPushButton("Stop", &playerWindow);

    buttons.addWidget(openFileButton);
    buttons.addWidget(playMediaButton);
    buttons.addWidget(pauseMeiaButton);
    buttons.addWidget(stopMediaButton);

    vbox.addLayout(&buttons);

    QString filePath;
    auto *audioOutput = new QAudioOutput;
    auto *player = new QMediaPlayer(&playerWindow);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
    bool isPlaying = false;

    QObject::connect(openFileButton, &QPushButton::clicked, [&filePath, player, &isPlaying, &playerWindow]()
    {
       filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "C:\\Qt\\Projects\\Less38_3", "mp3 files(*.mp3)");
       player->stop();
       isPlaying = false;
       playerWindow.setWindowTitle(filePath);
    });
    QObject::connect(playMediaButton, &QPushButton::clicked, [player, songSlider, &filePath, &isPlaying]()
    {
        if(!isPlaying)
        {
            player->setSource(QUrl::fromLocalFile(filePath));
            QObject::connect(player, &QMediaPlayer::durationChanged, [songSlider](qint64 duration)
            {
                songSlider->setMinimum(0);
                songSlider->setMaximum(duration);
            });
            QObject::connect(player, &QMediaPlayer::positionChanged, [songSlider](qint64 pos)
            {
                songSlider->setValue(pos);
            });
            isPlaying = true;
        }
        player->play();
    });
    playerWindow.setWindowTitle("Multimedia player");
    playerWindow.show();
    return a.exec();
}
