test:
	@make -s -C Header test
	@make -s -C RequestLine test
	@make -s -C Body test
	@make -s -C BodyChunked test
	@make -s -C Request test

debug: test

clean:
	@make -s -C Header clean
	@make -s -C Body clean
	@make -s -C BodyChunked clean
	@make -s -C RequestLine clean
	@make -s -C Request clean

fclean:
	@make -s -C Header fclean
	@make -s -C Body fclean
	@make -s -C BodyChunked fclean
	@make -s -C RequestLine fclean
	@make -s -C Request fclean

re: fclean test
