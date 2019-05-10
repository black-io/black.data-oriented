#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
	/**
		@brief	Basic part of composition.
		The specific parts of real composition do not have to be derived from this base. The composition mediator freely operate with any arbitrary type
		of composition part. But in some cases the composition part is forced to know its host. In such situation the composition part may be based on this type
		to simplify the access to its host.

		The `GetHost()` member-function is implemented in such way the constant host will be accessed from constant part and the mutable host may be accessed
		from mutable part.

		The basic part of composition is non-transferable object, which can't be moved or copied. The restriction is implemented due to nature of composition.
		The memory for composition parts is placed just inside of host, so the parts may be only created and destroyed while the host lifetime.
		Additionally, imaging the composition part is transferable, the huge mess may happen once the part is moved outside of host and
		requires the instance of host. The result may vary in wide range of failures. And the most dramatic is the result when the code will not fail immediately,
		but the memory will be corrupted.
		So, the user shell never consider the composition parts to be transferable in memory.

		And one more thing. It's strongly recommended to make the private inheritance from instantiations of this template.
		In my thoughts, the composition part should not be a polymorphic type.

		@tparam	THost	The type of host for specific composition part.
	*/
	template< typename THost >
	class CompositionPart : private Black::NonTransferable
	{
	// Construction.
	public:
		CompositionPart()	= delete;
		CompositionPart( const size_t host_offset ) : m_host_offset{ host_offset } {};

		~CompositionPart()	= default;

	// Heirs interface.
	protected:
		// Get the host object.
		inline THost& GetHost()					{ return *reinterpret_cast<THost*>( GetMemory() - m_host_offset ); };

		// Get the host object.
		inline const THost& GetHost() const		{ return *reinterpret_cast<const THost*>( GetMemory() - m_host_offset ); };

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
