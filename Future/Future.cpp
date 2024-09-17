#include <iostream>
#include<windows.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <future>

using namespace std;


void PromiseWorker(promise<string>&& promise) {
	promise.set_value("secret message");
}


int64_t Calculate() {
	int64_t sum = 0;

	for (int32_t i = 0; i < 100'000; i++) {
		sum += i;
	}
	return sum;
}

void TaskWorker(std::packaged_task<int64_t(void)>&& task) {
	task();
}

int main()
{
	//std::future
	{
		//1.deferrd -> 지연해서 실행하세요.
		//2.async -> 별도의 스레드를 만들어서 실행
		//3.deferred  | async -> 알아서 골라줘
		future<int64_t> future = async(launch::async, Calculate);

		//TODO
		/*std::future_status status = future.wait_for(1ms);

		if (status == future_status::ready) {잠시멈추고 확인
			//TODO
		}*/

		int64_t sum = future.get();

	}
	//std::promise
	{
		//future에 결과물을 반환해줄것이라는 약속?
		promise<string> promise;
		future<string> future = promise.get_future();

		thread t(PromiseWorker, move(promise));

		string message = future.get();
		cout << message << endl;
		
		t.join();


	}
	//std::package_task
	{
		packaged_task<int64_t(void)> task(Calculate);
		future<int64_t> future = task.get_future();

		thread t(TaskWorker, move(task));

		int64_t sum = future.get();
		cout << sum << endl;

		t.join();
	}

	//전부 비동기(async)실행 관련 stl 
	//한 번 발생하는 이벤트에 유용하다!
	//ex) 프로그램 실행시 설정파일json 또는 xml을 읽어오는데 자료가 너무 커서 읽는데 시간이 걸릴 경우 
	//async 비동기적으로 함수를 실행
	//promis 결과물을 promise를 통해 future로 받음
	//packaged_task 원하는 함수의 실행결과를 packaged_task를 통해 future로 받음
	//async외에는 잘 사용하지는않지만 공부한겸 정리

}
