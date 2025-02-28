test:
	@make -s -C Main test
	@make -s -C StressTest test

debug: test

clean:
	@make -s -C Main clean
	@make -s -C StressTest clean

fclean:
	@make -s -C Main fclean
	@make -s -C StressTest fclean

re: fclean test
