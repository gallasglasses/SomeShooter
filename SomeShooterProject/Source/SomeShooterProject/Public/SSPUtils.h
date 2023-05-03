#pragma once

class SSPUtils
{
	public:

	template<typename T>
	static T* GetPlayerComponent(APawn* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;
		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass()); 
		return Cast<T>(Component);
	}
};