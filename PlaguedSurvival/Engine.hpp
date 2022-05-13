class Engine
{
public:
	virtual ~Engine();
	static std::unique_ptr<Engine> sInstance;

	virtual int Run();
	void SetShouldKeepRunning(bool in_should_keep_running)
	{
		mShouldKeepRunning = in_should_keep_running;
	}
	virtual void HandleEvent(sf::Event& p_event);
	virtual bool PollEvent(sf::Event& p_event);

	virtual void ExternalDoFrame();
	void StartGame();

	bool GameStarted() const { return mGameStarted; }

protected:
	Engine();
	virtual void DoFrame();
	bool mGameStarted;

private:
	int DoRunLoop();
	bool mShouldKeepRunning;
};
