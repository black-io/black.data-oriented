#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	/// @brief	Associate the list of components with indices.
	template< typename... TComponents >
	class ComponentList final
	{
	// Public inner types and constants.
	public:
		/// @brief	Trivial list of components.
		using Components = typename ComponentCollection<TComponents...>;

		/// @brief	Length of component list.
		static constexpr const size_t LENGTH = sizeof...( TComponents );

	// Public static interface.
	public:
		/// @brief	Get the length of component list.
		static constexpr const size_t GetLength()	{ return LENGTH; };

		/// @brief	Get the index of component. `Black::UNDEFINED_INDEX` will be returned if `TComponent` not in list of components.
		template< typename TComponent >
		static constexpr const size_t GetIndex()	{ return typename ComponentIndexHelper<TComponent, Components, 0>::Find(); };
	};
}
}
}
}
