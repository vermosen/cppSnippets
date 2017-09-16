#include <string>
#include <algorithm>

int longestPalindrome(int n, std::string s)
{
	int i, j, k;

	// initial control over the input
	if (n > s.size()) return -1;

	if (n < s.size())
		s.erase(n, s.size() - n);

	// build the table
	int ** tbl = new int *[n];
	for (i = 0; i < n; ++i)
		tbl[i] = new int[n];

	for (i = 0; i < n; i++)
		tbl[i][i] = 1;

	for (k = 2; k <= n; k++)
	{
		for (i = 0; i < n - k + 1; i++)
		{
			j = i + k - 1;
			if (s[i] == s[j] && k == 2)
				tbl[i][j] = 2;
			else if (s[i] == s[j])
				tbl[i][j] = tbl[i + 1][j - 1] + 2;
			else
				tbl[i][j] = std::max(tbl[i][j - 1], tbl[i + 1][j]);
		}
	}

	return tbl[0][n - 1];
}

int main()
{
	std::string s("lfhdihfohf");

	int res = longestPalindrome(10, s);

	return 0;
}
