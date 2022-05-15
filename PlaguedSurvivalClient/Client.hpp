class Client : public Engine
{
public:

	static bool StaticInit();

	void ExternalDoFrame() override;

protected:

	Client();

	void DoFrame() override;
	void HandleEvent(sf::Event& p_event) override;
	bool PollEvent(sf::Event& p_event) override;
	void Reset();
};
