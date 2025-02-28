test:
	@make -s -C DirectoryListing test
	@make -s -C FileUpload test
	@make -s -C FileDownload test
	@make -s -C HttpErrorCodes test
	@make -s -C ValidateHeaders test
	@make -s -C ValidateAcceptType test
	@make -s -C CgiGateway test
	@make -s -C RequestResponseUpload test
	@make -s -C RequestResponseUploadBottleneck test

debug: test

clean:
	@make -s -C DirectoryListing clean
	@make -s -C FileUpload clean
	@make -s -C FileDownload clean
	@make -s -C HttpErrorCodes clean
	@make -s -C ValidateHeadersclean
	@make -s -C ValidateAcceptType clean
	@make -s -C CgiGatewayclean
	@make -s -C RequestResponseUpload clean
	@make -s -C RequestResponseUploadBottleneck clean

fclean:
	@make -s -C DirectoryListing fclean
	@make -s -C FileUpload fclean
	@make -s -C FileDownload fclean
	@make -s -C HttpErrorCodes fclean
	@make -s -C ValidateAcceptType ValidateHeadersclean
	@make -s -C ValidateAcceptType fclean
	@make -s -C ValidateAcceptType CgiGatewayclean
	@make -s -C RequestResponseUpload fclean
	@make -s -C RequestResponseUploadBottleneck fclean

re: fclean test
