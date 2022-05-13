class MusicPlayer : private sf::NonCopyable
{
public:
	static void StaticInit();

	static std::unique_ptr<MusicPlayer> sInstance;

public:
	void Play(MusicThemes theme);
	void Stop();

	void SetPaused(bool paused);
	void SetVolume(float volume);

protected:
	MusicPlayer();

private:
	sf::Music mMusic;
	std::map<MusicThemes, std::string>	mFileNames;
	float mVolume;
};