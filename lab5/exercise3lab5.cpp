#include <iostream>

class logical_values_array {
   private:
	unsigned int value = 0;
   public:
	logical_values_array(unsigned int number) {
		value = number;
	}
	logical_values_array() = default;
	unsigned int get_value() const{
		return value;
	}
	logical_values_array inversion() const {
		return {~value};
	}
	logical_values_array conjunction(const logical_values_array& right) const {
		return {value & right.get_value()};
	}
	logical_values_array disjunction(const logical_values_array& right) const {
		return {value | right.get_value()};
	}
	logical_values_array implication(const logical_values_array& right) const {
		return this->inversion().disjunction(right);
	}
	logical_values_array coimplication(const logical_values_array& right) const {
		return this->implication(right).inversion();
	}
	logical_values_array addition_module_2(const logical_values_array& right) const {
		return right.conjunction(this->inversion()).disjunction(this->conjunction(right));
	}
	logical_values_array equivalent(const logical_values_array& right) const {
		return this->addition_module_2(right).inversion();
	}
	logical_values_array pierce(const logical_values_array& right) const {
		return this->disjunction(right).inversion();
	}
	logical_values_array shaeffer(const logical_values_array& right) const {
		return this->conjunction(right).inversion();
	}
};