#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <deque>
# include <iostream>
# include <iomanip>
# include <sstream>

class PmergeMe {
	private:
		std::vector<int>	_vec;
		std::deque<int>		_deq;
		
		void    mergeSortVector(std::vector<int>& arr, int left, int right);
		void    mergeVector(std::vector<int>& arr, int left, int mid, int right);
		void    insertionSortVector(std::vector<int>& arr, int left, int right);
		
		void    mergeSortDeque(std::deque<int>& arr, int left, int right);
		void    mergeDeque(std::deque<int>& arr, int left, int mid, int right);
		void    insertionSortDeque(std::deque<int>& arr, int left, int right);
		
		template<typename Container>
		double  sortAndMeasureTime(Container& container);
		
		PmergeMe& operator=(const PmergeMe& other);

	public:
		PmergeMe(void);
		PmergeMe(const PmergeMe& other);
		~PmergeMe(void);

		bool    parseInput(int argc, char** argv);
		void    sort(void);
		void    displayBefore(void) const;
		void    displayAfter(void) const;
};

#endif