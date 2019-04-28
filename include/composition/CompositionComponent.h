#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
	//
	template< typename THost >
	class CompositionComponent : private Black::NonTransferable
	{
	// Construction.
	public:
		CompositionComponent( const size_t host_offset ) : m_host_offset{ host_offset } {};

	// Heirs interface.
	protected:
		// Get the host object.
		inline THost* GetHost()					{ return reinterpret_cast<THost*>( GetMemory() - m_host_offset ); };

		// Get the host object.
		inline const THost* GetHost() const		{ return reinterpret_cast<const THost*>( GetMemory() - m_host_offset ); };

	// Private interface.
	private:
		// Get the raw memory from this object.
		inline uint8_t* GetMemory()				{ return reinterpret_cast<uint8_t*>( this ); };

		// Get the raw memory from this object.
		inline const uint8_t* GetMemory() const	{ return reinterpret_cast<const uint8_t*>( this ); };

	// Private state.
	private:
		const size_t m_host_offset;	// Offset from host base to this component.
	};
}
}
}
