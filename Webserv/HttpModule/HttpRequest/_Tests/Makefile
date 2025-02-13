test:
	@make -s -C Header test
	# uncomment when body has tests @make -s -C Body test
	@make -s -C RequestLine test

debug: test

clean:
	@make -s -C Header clean
	# uncomment when body has tests @make -s -C Body clean
	@make -s -C RequestLine clean

fclean:
	@make -s -C Header fclean
	# uncomment when body has tests @make -s -C Body fclean
	@make -s -C RequestLine fclean

re: fclean test
