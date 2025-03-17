#include "timenotifier.h"
#include <QMetaObject>


TimeNotifier::TimeNotifier() : overlay(nullptr)
{
    //overlay = new OverlayManager();
}

TimeNotifier::~TimeNotifier()
{
    delete overlay;
}

void TimeNotifier::handleTimeEvent(char timeInGame[7])
{
    static const std::unordered_map<std::string, TimeEvent> events = {
      {"0:45", {"sound.mp3", "Check the spot for an enemy Sentry."}},
      {"27:17", {Sounds::Stack, "Make a stack"}},
      {"1:35", {Sounds::Stack, "Make a stack"}},
      {"2:30", {"sound.mp3", "Lotus at 3:00"}},
      {"3:35", {"sound.mp3", "Bounty rune at 4:00"}},
      {"4:00", {"sound.mp3", "Buy an observer for the night"}},
      {"4:35", {"sound.mp3", "Make a stack"}},
      {"4:58", {"sound.mp3", "Tier 1 neutral items at 5:00"}},
      {"5:20", {"sound.mp3", "Lotus at 6:00"}},
      {"5:30", {"sound.mp3", "Amplification rune at 6:00"}},
      {"5:35", {"sound.mp3", "Make a stack"}},
      {"6:10", {"sound.mp3", "Wisdom rune at 7:00"}},
      {"6:30", {"sound.mp3", "Wisdom rune at 7:00"}},
      {"6:35", {"sound.mp3", "Make a stack"}},
      {"7:20", {"sound.mp3", "Amplification rune at 8:00"}},
      {"7:30", {"sound.mp3", "Bounty rune at 8:00"}},
      {"7:40", {"sound.mp3", "Make a stack"}},
      {"8:40", {"sound.mp3", "Make a stack"}},
      {"9:30", {"sound.mp3", "Amplification rune at 10:00"}},
      {"9:40", {"sound.mp3", "Make a stack"}},
      {"10:40", {"sound.mp3", "Make a stack"}},
      {"11:30", {"sound.mp3", "Amplification rune at 12:00"}},
      {"11:40", {"sound.mp3", "Make a stack"}},
      {"12:40", {"sound.mp3", "Make a stack"}},
      {"13:10", {"sound.mp3", "Wisdom rune at 14:00"}},
      {"13:40", {"sound.mp3", "Make a stack"}},
      {"14:40", {"sound.mp3", "Make a stack"}},
      {"14:45", {"sound.mp3", "Tormentor at 15:00"}},
      {"14:55", {"sound.mp3", "Tier 2 neutral items at 15:00"}},
      {"3:10", {"sound.mp3", "Tier 3 neutral items at 25:00"}},
      {"0:03", {Sounds::Stack, "Text"}},
      {"4:45", {"sound.mp3", "Text2"}},

      {"34:55", {"sound.mp3", "Tier 4 neutral items at 35:00"}},
      {"59:15", {"sound.mp3", "Tier 5 neutral items at 60:00"}},
      };

    auto it = events.find(timeInGame);
    if (it != events.end()) {
        QMetaObject::invokeMethod(this, [this, it]() {
            playSound(it->second.soundFile);
            showTextOverlay(it->second.textOverlay);
        }, Qt::QueuedConnection);
    }
}

void TimeNotifier::playSound(const QString &file)
{

    SoundManager::playSound(file);
}

void TimeNotifier::showTextOverlay(const QString &text)
{
    if (overlay) {
        overlay->close();
        delete overlay;
    }
    overlay = new OverlayManager();
    overlay->setOverlayText(text);
    overlay->showOverlay();
}
