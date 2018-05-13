class MemoryUsageImpl : public MemoryUsage {
  public:
    MemoryUsage();
    ~MemoryUsage() {}
    uint64_t getVSS() const override;
    uint64_t getRSS() const override;
    uint64_t getPSS() const override;
    uint64_t getUSS() const override;
  private:
    pm_memusage_t m_MemUsage;
};