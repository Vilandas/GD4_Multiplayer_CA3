class DataTables
{
public:
	struct CharacterAnimationData
	{
		AnimationData idle;
		AnimationData run;
		AnimationData jump;

		std::vector<AnimationData> ToVector() const
		{
			return { idle, run, jump };
		}
	};

public:
	static constexpr CharacterAnimationData CharacterData = CharacterAnimationData{
		{Textures::kDocIdle, 190, 256, 10, 1.2f},
		{Textures::kDocRun, 256, 256, 4, 1},
		{Textures::kDocIdle, 190, 256, 10, 1}
	};
};