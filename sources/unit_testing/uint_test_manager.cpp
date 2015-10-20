
namespace reflective
{
	class UnitTesingManager::Impl
	{
		class Test
		{
		public:

			Test(const Test &) = delete;

			Test & operator = (const Test &) = delete;

			Test(TestFunction i_function)
				: m_function(std::move(i_function))
			{
			}

		private:
			std::chrono::duration<double> m_duration;
			TestFunction m_function;
		};

		class CorrectnessTest : public Test
		{
		public:
			CorrectnessTest(TestFunction i_function)
				: Test(std::move(i_function))
			{
			}
		};

		class PerformanceTest : public Test
		{
		public:
			PerformanceTest(TestFunction i_function, StringView i_version_label)
				: Test(std::move(i_function)), m_version_label(i_version_label.data(), i_version_label.size())
			{
			}

		private:
			std::string m_version_label;
		};

		class Node
		{		
		public:

			Node(std::string i_name) : m_name(std::move(i_name)) {}

			const std::string & name() const { return m_name; }

			void add_test(std::shared_ptr< Test > i_test)
			{
				m_tests.push_back(i_test);
			}

			const std::vector< std::shared_ptr< Test > > & tests() const { return m_tests; }

			const std::vector<Node> & children() const { return m_children; }

			Node * add_child(std::string i_name)
			{
				m_children.emplace_back(i_name);
				return &m_children.back();
			}

			Node * find_child(StringView i_name)
			{
				auto entry_it = Ext::find_if(m_children, [i_name](const Node & i_entry) { return i_name == i_entry.name().c_str(); });
				if (entry_it != m_children.end())
				{
					return &*entry_it;
				}
				else
				{
					return nullptr;
				}
			}

		private:
			std::string m_name;
			std::vector< std::shared_ptr< Test > > m_tests;
			std::vector<Node> m_children;
		};

		Node m_root;

	public:

		Impl()
			: m_root("")
		{

		}

		Node * find_entry(StringView i_path)
		{
			Node * node = &m_root;
			for_each_token(i_path, '/', [&node](StringView i_token) {

				if (node != nullptr)
				{
					node = node->find_child(i_token);
				}
			});

			return node;
		}

		Node & find_or_add_entry(StringView i_path)
		{
			Node * node = &m_root;
			for_each_token(i_path, '/', [&node](StringView i_token) {

				Node * child = node->find_child(i_token);

				if (child == nullptr)
				{
					node = node->add_child(std::string(i_token.data(), i_token.size()));
				}
				else
				{
					node = child;
				}
			});

			return *node;
		}

		void add_correctness_test(StringView i_path, TestFunction i_function)
		{
			find_or_add_entry(i_path).add_test(std::make_shared<CorrectnessTest>(i_function));
		}

		void add_performance_test(StringView i_path, TestFunction i_function, StringView i_version_label)
		{
			find_or_add_entry(i_path).add_test(std::make_shared<PerformanceTest>(i_function, i_version_label));
		}

		void run(StringView i_path)
		{
			auto node = find_entry(i_path);
			if (node != nullptr)
			{
				
			}
		}
	};

	UnitTesingManager & UnitTesingManager::instance()
	{
		static UnitTesingManager instance;
		return instance;
	}
	
	UnitTesingManager::UnitTesingManager()
		: m_impl( std::make_unique<Impl>() )
	{
	}

	void UnitTesingManager::add_correctness_test(StringView i_path, TestFunction i_function)
	{
		m_impl->add_correctness_test(i_path, i_function);
	}

	void UnitTesingManager::add_performance_test(StringView i_path, TestFunction i_function, StringView i_version_label)
	{
		m_impl->add_performance_test(i_path, i_function, i_version_label);
	}

	void UnitTesingManager::run(StringView i_path)
	{
		m_impl->run(i_path);
	}
}