test:
	@make -s -C DirectoryListing test
	@make -s -C FileUpload test
	@make -s -C HttpErrorCodes test
	@make -s -C ParseAcceptHeader test
	@make -s -C RequestResponseUpload test
	@make -s -C RequestResponseUploadBottleneck test

debug: test

clean:
	@make -s -C DirectoryListing clean
	@make -s -C FileUpload clean
	@make -s -C HttpErrorCodes clean
	@make -s -C ParseAcceptHeader clean
	@make -s -C RequestResponseUpload clean
	@make -s -C RequestResponseUploadBottleneck clean

fclean:
	@make -s -C DirectoryListing fclean
	@make -s -C FileUpload fclean
	@make -s -C HttpErrorCodes fclean
	@make -s -C ParseAcceptHeader fclean
	@make -s -C RequestResponseUpload fclean
	@make -s -C RequestResponseUploadBottleneck fclean

re: fclean test
