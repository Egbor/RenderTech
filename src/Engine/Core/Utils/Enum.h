#ifndef ENUM_H
#define ENUM_H

#include <type_traits>

namespace Engine {
	template<typename TEnumBit>
	class EnumFlags {
	public:
		using TMask = typename std::underlying_type<TEnumBit>::type;

		constexpr EnumFlags() noexcept : m_mask(0) {}
		constexpr EnumFlags(TEnumBit bit) noexcept : m_mask(static_cast<TMask>(bit)) {}
		constexpr explicit EnumFlags(TMask flags) noexcept : m_mask(flags) {}

		constexpr EnumFlags<TEnumBit> operator&(const EnumFlags<TEnumBit>& flags) const noexcept {
			return EnumFlags<TEnumBit>(m_mask & flags.m_mask);
		}

		constexpr EnumFlags<TEnumBit> operator|(const EnumFlags<TEnumBit>& flags) const noexcept {
			return EnumFlags<TEnumBit>(m_mask | flags.m_mask);
		}

	private:
		TMask m_mask;
	};

	template<typename TEnumBit>
	constexpr EnumFlags<TEnumBit> operator&(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator&(bit);
	}

	template<typename TEnumBit>
	constexpr EnumFlags<TEnumBit> operator|(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator|(bit);
	}

	template<typename TEnumBit>
	inline constexpr EnumFlags<TEnumBit> operator&(TEnumBit lbit, TEnumBit rbit) noexcept {
		return EnumFlags<TEnumBit>(lbit) & rbit;
	}

	template<typename TEnumBit>
	inline constexpr EnumFlags<TEnumBit> operator|(TEnumBit lbit, TEnumBit rbit) noexcept {
		return EnumFlags<TEnumBit>(lbit) | rbit;
	}
}

#endif // !ENUM_H
