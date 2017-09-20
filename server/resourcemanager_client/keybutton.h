#ifndef KEYBUTTON_H
#define KEYBUTTON_H

#include <QPushButton>
#include <QTimer>

class KeyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit KeyButton(QString text, QWidget *parent = 0) : QPushButton(text, parent),
        pauseMsecs(400), intervalMsecs(30)
    {
        tm = new QTimer(this);
        setText(text);
        connect(tm, SIGNAL(timeout()), this, SLOT(on_pressed_last()));
        connect(this, SIGNAL(pressed()), this, SLOT(on_pressed()));
        connect(this, SIGNAL(released()), this, SLOT(on_released()));
        connect(this, SIGNAL(clicked()), this, SLOT(on_clicked()));
    }

private:
    QTimer *tm;
    long pauseMsecs;
    long intervalMsecs;

signals:
    void keyPressed(const QString &msg);
    void keyReleased(const QString &msg);
    void keyClicked(const QString &msg);

public slots:
    void on_pressed() { emit this->keyPressed(this->text());
                        tm->start(pauseMsecs); }
    void on_pressed_last() { emit this->keyPressed(this->text());
                             tm->setInterval(intervalMsecs); }
    void on_released() { tm->stop(); emit this->keyReleased(this->text()); }
    void on_clicked() { emit this->keyClicked(this->text()); }
    
};

#endif // KEYBUTTON_H
