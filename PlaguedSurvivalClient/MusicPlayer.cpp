#include "RoboCatClientPCH.hpp"

std::unique_ptr<MusicPlayer> MusicPlayer::sInstance;

void MusicPlayer::StaticInit()
{
	sInstance.reset(new MusicPlayer());
}


MusicPlayer::MusicPlayer()
	: mVolume(100.f)
{
	mFileNames[MusicThemes::kMenuTheme] = "../Assets/Music/Plagued_Choir.ogg";
}

void MusicPlayer::Play(MusicThemes theme)
{
	std::string filename = mFileNames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::Stop()
{
	mMusic.stop();
}

void MusicPlayer::SetVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::SetPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}