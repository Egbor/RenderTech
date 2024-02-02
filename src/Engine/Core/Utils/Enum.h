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
		constexpr EnumFlags(const EnumFlags<TEnumBit>& flags) noexcept = default;
		constexpr explicit EnumFlags(TMask flags) noexcept : m_mask(flags) {}

		// Relational operators

		constexpr bool operator<(const EnumFlags<TEnumBit>& flags) const noexcept {
			return m_mask < flags.m_mask;
		}

		constexpr bool operator<=(const EnumFlags<TEnumBit>& flags) const noexcept {
			return m_mask <= flags.m_mask;
		}

		constexpr bool operator>(const EnumFlags<TEnumBit>& flags) const noexcept {
			return m_mask > flags.m_mask;
		}

		constexpr bool operator>=(const EnumFlags<TEnumBit>& flags) const noexcept {
			return m_mask >= flags.m_mask;
		}

		constexpr bool operator==(const EnumFlags<TEnumBit>& flags) const noexcept {
			return m_mask == flags.m_mask;
		}

		constexpr bool operator!=(const EnumFlags<TEnumBit>& flags) const noexcept {
			return m_mask != flags.m_mask;
		}

		// Logical operators

		constexpr bool operator!() const noexcept {
			return !m_mask;
		}

		// Bitwise operators

		constexpr EnumFlags<TEnumBit> operator&(const EnumFlags<TEnumBit>& flags) const noexcept {
			return EnumFlags<TEnumBit>(m_mask & flags.m_mask);
		}

		constexpr EnumFlags<TEnumBit> operator|(const EnumFlags<TEnumBit>& flags) const noexcept {
			return EnumFlags<TEnumBit>(m_mask | flags.m_mask);
		}

		constexpr EnumFlags<TEnumBit> operator^(const EnumFlags<TEnumBit>& flags) const noexcept {
			return EnumFlags<TEnumBit>(m_mask ^ flags.m_mask);
		}

		// Assignment operators

		constexpr EnumFlags<TEnumBit>& operator&=(const EnumFlags<TEnumBit>& flags) const noexcept {
			m_mask &= flags.m_mask;
			return *this;
		}

		constexpr EnumFlags<TEnumBit>& operator|=(const EnumFlags<TEnumBit>& flags) const noexcept {
			m_mask |= flags.m_mask;
			return *this;
		}

		constexpr EnumFlags<TEnumBit>& operator^=(const EnumFlags<TEnumBit>& flags) const noexcept {
			m_mask ^= flags.m_mask;
			return *this;
		}

		// Cast operators

		explicit constexpr operator bool() const noexcept {
			return !!m_mask;
		}

		explicit constexpr operator TMask() const noexcept {
			return m_mask;
		}

	private:
		TMask m_mask;
	};

	template<typename TEnumBit>
	constexpr bool operator<(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator<(bit);
	}

	template<typename TEnumBit>
	constexpr bool operator<=(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator<=(bit);
	}

	template<typename TEnumBit>
	constexpr bool operator>(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator>(bit);
	}

	template<typename TEnumBit>
	constexpr bool operator>=(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator>=(bit);
	}

	template<typename TEnumBit>
	constexpr bool operator==(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator==(bit);
	}

	template<typename TEnumBit>
	constexpr bool operator!=(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator!=(bit);
	}

	template<typename TEnumBit>
	constexpr EnumFlags<TEnumBit> operator&(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator&(bit);
	}

	template<typename TEnumBit>
	constexpr EnumFlags<TEnumBit> operator|(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator|(bit);
	}

	template<typename TEnumBit>
	constexpr EnumFlags<TEnumBit> operator^(TEnumBit bit, const EnumFlags<TEnumBit>& flags) noexcept {
		return flags.operator^(bit);
	}

	template<typename TEnumBit>
	inline constexpr EnumFlags<TEnumBit> operator&(TEnumBit lbit, TEnumBit rbit) noexcept {
		return EnumFlags<TEnumBit>(lbit) & rbit;
	}

	template<typename TEnumBit>
	inline constexpr EnumFlags<TEnumBit> operator|(TEnumBit lbit, TEnumBit rbit) noexcept {
		return EnumFlags<TEnumBit>(lbit) | rbit;
	}

	template<typename TEnumBit>
	inline constexpr EnumFlags<TEnumBit> operator^(TEnumBit lbit, TEnumBit rbit) noexcept {
		return EnumFlags<TEnumBit>(lbit) ^ rbit;
	}
}

#endif // !ENUM_H
