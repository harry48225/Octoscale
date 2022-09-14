namespace BLE {
  bool isDeviceConnected();
  void init();
  void update(float mass);
  bool isPendingTare();
  void clearPendingTare();
  void startTiming();
  void stopTiming();
  void updateTimerDuration(long seconds);
  bool isPendingAButton();
  void clearPendingAButton();
  bool isPendingBButton();
  void clearPendingBButton();
}