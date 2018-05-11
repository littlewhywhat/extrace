class MemoryUsageImpl : public MemoryUsage {
  public:
    MemoryUsage();
    ~MemoryUsage() {}
    size_t getVSS() const override;
    size_t getRSS() const override;
    size_t getPSS() const override;
    size_t getUSS() const override;
  private:
    pm_memusage_t m_MemUsage;
};