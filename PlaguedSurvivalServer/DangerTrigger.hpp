/**
 * Vilandas Morrissey - D00218436
 */

class DangerTrigger
{
public:
	static void StaticInit();
	static std::unique_ptr<DangerTrigger> sInstance;

	void Clear();
	void AddDangerObject(Dangerous* danger);
	void RemoveDangerObject(const Dangerous* danger);
	void Update(float inDeltaTime);

protected:
	DangerTrigger();

	const float DEFAULT_DPS = 0.8f;
	const float DEFAULT_IPS = 0.1f;

	std::vector<Dangerous*> mDangers;
	float mDangerTime;
	float mDangersPerSecond;
	float mDangerIncrementPerSecond;
};