namespace Timer {
  void stop();
  void start();
  void startWithCountDown(int countdownDurationSeconds);

  unsigned long getSecondsElapsed();
}