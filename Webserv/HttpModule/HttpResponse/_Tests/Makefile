test:
	@make -s -C FileUpload test
	@make -s -C FileDownload test
	@make -s -C HttpErrorCodes test
	@make -s -C ResolveRequestData test
	@make -s -C ValidateAcceptType test
	@make -s -C RequestResponseUpload test
	@make -s -C RequestResponseUploadBottleneck test

debug: test

clean:
	@make -s -C FileUpload clean
	@make -s -C FileDownload clean
	@make -s -C HttpErrorCodes clean
	@make -s -C ResolveRequestData clean
	@make -s -C ValidateAcceptType clean
	@make -s -C RequestResponseUpload clean
	@make -s -C RequestResponseUploadBottleneck clean

fclean:
	@make -s -C FileUpload fclean
	@make -s -C FileDownload fclean
	@make -s -C HttpErrorCodes fclean
	@make -s -C ResolveRequestData fclean
	@make -s -C ValidateAcceptType fclean
	@make -s -C RequestResponseUpload fclean
	@make -s -C RequestResponseUploadBottleneck fclean

re: fclean test
