
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../HttpRequest/HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"

size_t g_maxFileWrite = 10;

static const char potato_text[] = "Potatoes: A Versatile and Nutritious Staple\n"
"The potato (Solanum tuberosum) is one of the most widely consumed and cultivated crops in the world. "
"Originally domesticated in the Andean region of South America over 7,000 years ago, potatoes have since "
"spread across the globe, becoming a dietary staple in many cultures. Their resilience, nutritional value, "
"and versatility in cooking make them one of the most important agricultural products today."
"\n\n"
"Origins and Cultivation"
"Potatoes were first cultivated by indigenous peoples in present-day Peru and Bolivia. Spanish explorers "
"brought them to Europe in the 16th century, where they quickly gained popularity due to their ability to "
"grow in diverse climates. By the 18th century, potatoes had become a key food source in many European "
"countries, particularly in Ireland, where they became a staple crop. However, reliance on a single variety "
"led to the devastating Irish Potato Famine (1845–1852) when a fungal disease known as potato blight wiped "
"out crops, causing mass starvation."
"\n\n"
"Today, potatoes are grown in over 150 countries. They thrive in cool climates with well-drained soil and "
"require relatively little maintenance compared to other crops. China, India, Russia, and the United States "
"are among the largest producers, supplying both local and global markets."
"\n\n"
"Nutritional Benefits"
"Despite misconceptions that potatoes are unhealthy, they are actually packed with essential nutrients. "
"A medium-sized potato provides a good source of vitamin C, vitamin B6, potassium, and fiber—especially "
"when eaten with the skin. They are naturally fat-free and contain complex carbohydrates, which provide "
"long-lasting energy. While deep-fried preparations like French fries and potato chips add unhealthy "
"fats and excess sodium, boiled, baked, or roasted potatoes can be a nutritious addition to any diet."
"\n\n"
"Varieties of Potatoes"
"There are thousands of potato varieties, each suited for different culinary uses. Broadly, they can be "
"categorized into three types:"
"\n\n"
"Starchy Potatoes – These have a high starch content and a dry, fluffy texture, making them ideal for "
"baking, frying, and mashing. Russet potatoes are the most well-known example."
"Waxy Potatoes – These have a lower starch content and hold their shape well when cooked, making them "
"perfect for boiling, roasting, and using in salads. Red potatoes and fingerlings fall into this category."
"All-Purpose Potatoes – These strike a balance between starchy and waxy, meaning they can be used in a "
"variety of dishes. Yukon Gold potatoes are a popular all-purpose variety."
"Culinary Uses"
"Potatoes are incredibly versatile in the kitchen and are used in cuisines around the world. Some of" 
"the most famous potato-based dishes include mashed potatoes, French fries, potato salad, gnocchi, latkes, "
"and shepherd’s pie. In addition to savory dishes, potatoes can also be used in bread and even desserts, such as potato-based doughnuts."
"\n\n"
"Conclusion"
"The humble potato is a vital food crop with a fascinating history and countless culinary applications. "
"Whether boiled, baked, mashed, or fried, potatoes continue to be a beloved and indispensable ingredient "
"in kitchens worldwide. Their nutritional benefits, affordability, and adaptability ensure that they will "
"remain a dietary staple for generations to come.";

static const char orange_text[] = "Oranges: A Juicy and Nutritious Citrus Fruit"
"\n\n"
"The orange (Citrus × sinensis) is one of the most popular and widely cultivated "
"fruits in the world. Known for its refreshing taste, vibrant color, and high "
"vitamin C content, oranges are enjoyed fresh, juiced, and in a variety of culinary "
"applications. Originating in Asia, oranges have spread across the globe, becoming "
"a staple in many diets due to their health benefits and versatility."
"\n\n"
"Origins and Cultivation"
"Oranges are believed to have originated in southern China, northeastern India, "
"and Southeast Asia thousands of years ago. They were introduced to the Mediterranean "
"region around 200 BCE and later brought to the Americas by Spanish explorers in the "
"15th and 16th centuries. Today, oranges are grown in warm, subtropical, and tropical "
"climates, with Brazil, the United States, China, and India being among the largest "
"producers."
"\n\n"
"Orange trees thrive in well-drained, slightly acidic soil with plenty of sunlight. "
"They are evergreen, meaning they keep their leaves year-round, and can produce fruit "
"for several decades. The fruit typically takes several months to ripen, turning from "
"green to bright orange as it matures, although some varieties remain green even when "
"fully ripe."
"\n\n"
"Nutritional Benefits"
"Oranges are well known for their high vitamin C content, which supports immune "
"function and acts as a powerful antioxidant. They also provide dietary fiber, potassium,"
" folate, and other essential vitamins and minerals. Oranges are naturally low in calories "
"and contain beneficial plant compounds such as flavonoids and carotenoids, which have been"
" linked to heart health and reduced inflammation."
"\n\n"
"Consuming oranges regularly can help improve digestion, support healthy skin, and "
"lower the risk of chronic diseases such as heart disease and high blood pressure. "
"Orange juice is a popular way to enjoy the fruit’s nutrients, but eating whole oranges"
" is even healthier due to the fiber content found in the pulp."
"\n\n"
"Varieties of Oranges"
"There are many different varieties of oranges, each with unique characteristics "
"suited for different purposes. The main types include:"
"\n\n"
"Sweet Oranges – The most commonly consumed variety, used for fresh eating and juicing."
" Examples include Navel oranges, Valencia oranges, and Blood oranges."
"Bitter Oranges – These are less commonly eaten fresh due to their tart flavor but are "
"used to make marmalades, liqueurs (such as Grand Marnier), and traditional medicines. "
"The Seville orange is a well-known example."
"Mandarins and Related Varieties – These include smaller, easy-to-peel citrus fruits "
"like tangerines, clementines, and satsumas. They are typically sweeter and less acidic"
" than standard oranges."
"Culinary Uses"
"Oranges are used in a wide range of culinary applications. Fresh oranges can be eaten"
" on their own, added to fruit salads, or used as a garnish. Orange juice is a breakfast"
"  staple, and its zest adds flavor to baked goods, marinades, and sauces. Oranges are "
"  also used in savory dishes, pairing well with meats, seafood, and salads. In some "
"  cuisines, dried orange peel is incorporated into spice blends and teas."
"\n\n"
"Conclusion"
"Oranges are a delicious and nutritious fruit with a rich history and global appeal. "
"Their refreshing taste, health benefits, and versatility make them a favorite among "
"people of all ages. Whether eaten fresh, juiced, or used in cooking, oranges continue "
"to be a cherished and essential part of a balanced diet.";



static const char* file1_Name = "test1.txt";
static const char* file1_Content = potato_text;
static const size_t file1_ContentSize = sizeof(potato_text) - 1;

static const char* file2_Name = "test2.txt";
static const char* file2_Content = orange_text;
static const size_t file2_ContentSize = sizeof(orange_text) - 1;

void fileUploadBottleNeck(int& testNumber, size_t readBufSize, size_t maxFileWrite)
{
	TEST_INTRO(testNumber++);
	ServerContext context;

	g_maxFileWrite = maxFileWrite;

	::unlink(file1_Name);
	::unlink(file2_Name);

	try
	{
		Http::Response 		response(context);
		Http::Request 		request(context);

		request.setResponse(response);

		///////////////////

		// manually send the request
		std::string boundary = "----WebKitFormBoundary12345";
		std::string contentDisp1 = "Content-Disposition: form-data; name=\"file1\"; filename=\"" + std::string(file1_Name) + "\"";
		std::string contentDisp2 = "Content-Disposition: form-data; name=\"file2\"; filename=\"" + std::string(file2_Name) + "\"";

		// placing file2 twice intentionally, ensure it is truncating properly
		std::string requestBodyMultipart = 
			"--" + boundary + "\r\n"
			+ contentDisp1 + "\r\n"
			"\r\n"
			+ file1_Content + "\r\n"
			"--" + boundary + "\r\n"
			+ contentDisp2 + "\r\n"
			"\r\n"
			+ file2_Content + "\r\n"
			"--" + boundary + "\r\n"
			+ contentDisp2 + "\r\n"
			"\r\n"
			+ file2_Content + "\r\n"
			"--" + boundary +
			"--\r\n";


		std::string requestHeader = 
		"POST /upload HTTP/1.1\r\n"
		"Host: example.com\r\n"
		"Content-Type: multipart/form-data; boundary=" + boundary + "\r\n"
		"Content-Length: " + TestHelpers::to_string(requestBodyMultipart.size()) + "\r\n"
		"\r\n";
	
		std::string fullRequest = requestHeader + requestBodyMultipart;

		///////////////

		HeapBuffer bufferRequest(fullRequest.size() + 100);
		HeapBuffer readBuffer(readBufSize);
		HeapBuffer testBuffer(std::max(file1_ContentSize, file2_ContentSize) + 100);
		
		bufferRequest.push(fullRequest.c_str(), fullRequest.size());

        while (bufferRequest.size() && request.getStatus() == Http::Status::OK)
        {
			int thisPush = readBuffer.available() < bufferRequest.size() ? readBuffer.available() : bufferRequest.size();
            readBuffer.push(BufferView(bufferRequest.data(), thisPush));
            bufferRequest.truncatePush(BufferView(bufferRequest.data() + thisPush, bufferRequest.size() - thisPush));

            // parse, tell the buffer to put the unconsumed part at the beginning
            readBuffer.truncatePush(request.parse(readBuffer));
        }


		int fd1 = ::open(file1_Name, O_RDONLY);
		testBuffer.read(fd1);
		::close(fd1);
		EXPECT_EQUAL(testBuffer.view(), BufferView(file1_Content, file1_ContentSize), "File content should match");

		int fd2 = ::open(file2_Name, O_RDONLY);
		testBuffer.read(fd2);
		::close(fd2);
		EXPECT_EQUAL(testBuffer.view(), BufferView(file2_Content, file2_ContentSize), "File content should match");

		EXPECT_EQUAL(request.getStatus(), Http::Status::OK, "Request status should be OK");

		TEST_PASSED_MSG("FileUpload Bottleneck Request-Response integration"
			", read buffer size: " + TestHelpers::to_string(readBufSize) 
			+ ", maxFileWrite: " + TestHelpers::to_string(maxFileWrite));
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	::unlink(file1_Name);
	::unlink(file2_Name);

}



int main(void)
{

	TEST_HEADER("Request to Response - File Upload");



	int testNumber = 1;

	// force File class to read slow and probably lower than what it is passed
	 for (size_t i = 200; i < 500; i += 1)
	 	fileUploadBottleNeck(testNumber, i, i / 2);

	TEST_FOOTER;

	return (0);
}