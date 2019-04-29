/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

// C++17 �Ȃ� fold expression �g����񂾂���
template<size_t I, typename... Ts>
class expand_tuplewise_grid_helper
{
public:
	static auto expand(const std::tuple<Ts...>& t)
	{
		return expand_tuplewise_grid_helper<I - 1, Ts...>::expand(t) * data::make(std::get<I - 1>(t));
	}
};

template<typename... Ts>
class expand_tuplewise_grid_helper<(size_t)1, Ts...>
{
public:
	static auto expand(const std::tuple<Ts...>& t)
	{
		return data::make(std::get<0>(t));
	}
};

template<typename... Ts>
auto expand_tuplewise_grid(const std::tuple<Ts...>& t)
{
	return expand_tuplewise_grid_helper<sizeof...(Ts), Ts...>::expand(t);
}

template<typename T>
struct decontainer
{
	typedef typename std::remove_reference<T>::type TT;
	struct value_type_helper
	{
		typedef typename std::remove_extent<TT>::type value_type;
	};

	typedef typename std::add_lvalue_reference<typename std::add_const<typename std::conditional<boost::unit_test::is_forward_iterable<TT>::value, TT, value_type_helper>::type::value_type>::type>::type type;
};

template<typename T>
struct tuple_expand;

template <typename ...Ts>
struct tuple_expand<std::tuple<Ts...>>
{
	typedef std::tuple<typename decontainer<Ts>::type...> type;
};

template<typename DS> // DS: �����̃f�[�^�Z�b�g�N���X
class expander
{
	typedef typename boost::decay<DS>::type ds_decay;
	/*
	* DS::sample �� tuple<A, vector<B>, vector<vector<C>> �ł���Ƃ��A
	* expander<DS>::sample �� tuple<A, B, vector<C>> �ł���B
	*
	* arity �͓���
	* tuple �̊e�v�f�^�ɂ��āA
	*   �R���e�i�^�i�R���e�i��z��j�Ȃ�R���e�i��1�i�K�O��
	*   �V���O���g���^�Ȃ炻�̂܂�
	*/
	/*
	* �����f�[�^�Z�b�g�͂Ƃ肠�����T�|�[�g�O
	*/
public:
	typedef typename tuple_expand<typename ds_decay::iterator::iterator_sample>::type sample;
	typedef typename vector<sample>::const_iterator container_iterator;

	explicit expander(DS&& ds) : m_src(std::forward<DS>(ds))
	{
		auto itr = m_src.begin();
		for (data::size_t i = 0; i < m_src.size(); ++i, ++itr)
		{
			auto e = *itr;
			auto expanded = expand_tuplewise_grid(e);
			auto jtr = expanded.begin();
			for (data::size_t j = 0; j < expanded.size(); ++j, ++jtr)
				m_dst.push_back(*jtr);
		}
	}

	expander(expander&& e) :
		m_src(std::forward<DS>(e.m_src)),
		m_dst(std::forward<vector<sample> >(e.m_dst))
	{
	}

	enum { arity = ds_decay::arity };

	class iterator
	{
	public:
		typedef sample iterator_sample;

		explicit iterator(container_iterator itr) : m_itr(itr) {}

		void operator++() { ++m_itr; }
		sample operator*() const { return *m_itr; }

	private:
		container_iterator m_itr;
	};

	data::size_t size() const { return m_dst.size(); }
	iterator begin() const { return iterator(m_dst.begin()); }

private:
	DS m_src;
	vector<sample> m_dst;
};

namespace boost {
	namespace unit_test {
		namespace data {
			namespace monomorphic {
				template<typename DS>
				struct is_dataset<expander<DS>> : mpl::true_ {};
			}
		}
	}
}

template<typename DS>
inline typename std::enable_if<data::monomorphic::is_dataset<DS>::value, expander<DS> >::type
expand(DS&& ds)
{
	return expander<DS>(std::forward<DS>(ds));
}
