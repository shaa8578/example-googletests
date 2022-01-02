#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>

//------------------------------------------------------------------------------
struct Driver {
  explicit Driver(const std::string& /*portName*/) {}
  virtual ~Driver() = default;

  virtual bool isAvailable() const = 0;
  virtual bool open() = 0;
  virtual void close() = 0;

  virtual void write(const std::string& /*data*/) = 0;
  virtual void setPortName(const std::string& /*portName*/) = 0;
};

//------------------------------------------------------------------------------
struct MockDriver : Driver {
  MockDriver() : Driver(std::string()) {
    ON_CALL(*this, isAvailable).WillByDefault([]() { return true; });
  }

  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(bool, open, (), (override));
  MOCK_METHOD(void, close, (), (override));
  MOCK_METHOD(void, write, (const std::string&), (override));

  MOCK_METHOD(void, setPortName, (const std::string&), (override));
};

//------------------------------------------------------------------------------
class BusnesLogic {
 public:
  explicit BusnesLogic(std::unique_ptr<Driver> driver)
      : m_driver(std::move(driver)) {}
  ~BusnesLogic() = default;

  void doSomething() {
    if (m_driver->isAvailable() == false) {
      throw std::domain_error("Driver not available");
    }

    if (m_driver->open() == false) {
      throw std::domain_error("Driver not opened");
    }

    m_driver->write(std::string("do_something"));

    m_driver->close();
  }

  void setupDriver() { m_driver->setPortName("new_port_name"); }

 private:
  std::unique_ptr<Driver> m_driver;
};

//------------------------------------------------------------------------------
class Test_BusnesLogic : public ::testing::Test {
 protected:
  virtual void SetUp() {
    auto drv = std::make_unique<MockDriver>();
    mock_driver = drv.get();
    busnes_logic = std::make_unique<BusnesLogic>(std::move(drv));
  }

  virtual void TearDown() {
    busnes_logic.reset(nullptr);
    mock_driver = nullptr;
  }

  MockDriver* mock_driver;
  std::unique_ptr<BusnesLogic> busnes_logic;
};

//------------------------------------------------------------------------------
using ::testing::Return;

//------------------------------------------------------------------------------
TEST_F(Test_BusnesLogic, doSomething_not_available) {
  EXPECT_CALL(*mock_driver, isAvailable()).WillOnce(Return(false));

  ASSERT_THROW(busnes_logic->doSomething(), std::domain_error);
}

//------------------------------------------------------------------------------
TEST_F(Test_BusnesLogic, doSomething_not_opened) {
  EXPECT_CALL(*mock_driver, open()).WillOnce(Return(false));

  ASSERT_THROW(busnes_logic->doSomething(), std::domain_error);
}

//------------------------------------------------------------------------------
using ::testing::Eq;

//------------------------------------------------------------------------------
TEST_F(Test_BusnesLogic, doSomething) {
  EXPECT_CALL(*mock_driver, open()).WillOnce(Return(true));
  EXPECT_CALL(*mock_driver, close()).Times(1);

  const std::string expected_data("do_something");
  EXPECT_CALL(*mock_driver, write(Eq(expected_data))).Times(1);

  ASSERT_NO_THROW(busnes_logic->doSomething());
}

//------------------------------------------------------------------------------
using ::testing::_;

//------------------------------------------------------------------------------
TEST_F(Test_BusnesLogic, setupDriver) {
  EXPECT_CALL(*mock_driver, setPortName(_)).Times(1);
  ASSERT_NO_THROW(busnes_logic->setupDriver());
}
