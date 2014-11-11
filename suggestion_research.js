var JSONTXT = 
        '{ "QueryTemplate": [' + 
            '{"templateName": "TagQuery1"}, ' + 
            '{"templateName": "TagQuery2"}, ' + 
            '{"templateName": "SQLQuery1"}, ' + 
            '{"templateName": "SQLQuery2"}, ' + 
            '{"templateName": "Query3"}, ' + 
            '{"templateName": "CoreQuery"}' + 
        ']}';

function kmp(dataSet, pattern) {
	// Exception case
	if (pattern == null || dataSet == null) return false;
	
	// Extra consideration	
	if (pattern.length === 0) return true;

	// Calculate the NEXT array
	var next = [0]; // First place must be 0 
	for (var i = 1; i != pattern.length; i++) {
		var cur = pattern[i];
		for (var j = i + 1; j != pattern.length; j++) {
			if (pattern[j] == cur)  
		}
	}

	for (i in dataSet) {

	}	
}

(function kmpTestDriver() {
	var resultSet = kmp(JSON.parese(JSONTXT), 'Qu');
	for (i in resultSet) {
		console.log(resultSet[i]);
	}
})();