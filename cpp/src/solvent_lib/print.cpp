#include <solvent_lib/print.hpp>
#include <solvent_util/str.hpp>

#include <iostream>


namespace solvent::lib::print {

	//
	void value(std::ostream& os, const solvent::Order O, const uint8_t value) {
		if (value == O*O) [[unlikely]] {
			os << ' ';
		} else {
			if (O < 4) {
				os << static_cast<unsigned>(value);
			} else if (O == 5) {
				os << static_cast<char>('a' + value);
			} else {
				(value < 10)
					? os << static_cast<unsigned>(value)
					: os << static_cast<char>('a' + value - 10);
			}
		}
	}


	void serial(std::ostream& os, const solvent::Order O, grid_t grid_view) {
		for (unsigned coord = 0; coord < O*O*O*O; coord++) {
			print::value(os, O, grid_view(coord));
		}
	}


	void pretty(std::ostream& os, const Order O, std::vector<grid_t> const& grid_views) {
		namespace str = solvent::util::str;
		using ord2_t = std::uint16_t;

		const auto print_blk_row_sep_string_ = [&os, O](const unsigned border_i) -> void {
			#define M_NOOK(NOOK_T, NOOK_C, NOOK_B) \
			if      (border_i == 0) { os << NOOK_T; } \
			else if (border_i == O) { os << NOOK_B; } \
			else                    { os << NOOK_C; }
			M_NOOK(" ┌", " ├", " └")
			for (unsigned blk_col = 0; blk_col < O; blk_col++) {
				for (unsigned i = 0; i < 1u + (2u * O); i++) {
					os << "─";
				}
				if (blk_col < O - 1u) { M_NOOK("┬", "┼", "┴") }
			}
			M_NOOK("┐", "┤", "┘")
			#undef M_NOOK
		};

		auto print_blk_row_sep_strings = [&](const unsigned border_i) mutable {
			os << '\n';
			print_blk_row_sep_string_(border_i);
			for (unsigned i = 1; i < grid_views.size(); i++) {
				os << "   ";
				print_blk_row_sep_string_(border_i);
			}
		};

		os << str::DIM.ON;
		for (ord2_t row = 0; row < O*O; row++) {
			if (row % O == 0) {
				print_blk_row_sep_strings(row / O);
			}
			os << '\n';
			for (unsigned grid_i = 0; grid_i < grid_views.size(); grid_i++) {
				for (ord2_t col = 0; col < O*O; col++) {
					if ((col % O) == 0) { os << str::DIM.ON << " │" << str::DIM.OFF; }
					os << ' ';
					print::value(os, O, grid_views[grid_i](row * O*O + col));
				}
				os << str::DIM.ON << " │";
				if (grid_i != grid_views.size() - 1) {
					os << "   ";
				}
			}
		}
		print_blk_row_sep_strings(O);
		os << str::DIM.OFF;
	}
}