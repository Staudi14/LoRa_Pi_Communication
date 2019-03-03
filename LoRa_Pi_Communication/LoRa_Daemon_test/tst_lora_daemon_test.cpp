#include <QtTest>

// add necessary includes here

class LoRa_Daemon_Test : public QObject
{
Q_OBJECT

public:
LoRa_Daemon_Test();
~LoRa_Daemon_Test();

private slots:
void test_case1();

};

LoRa_Daemon_Test::LoRa_Daemon_Test()
{

}

LoRa_Daemon_Test::~LoRa_Daemon_Test()
{

}

void LoRa_Daemon_Test::test_case1()
{

}

QTEST_APPLESS_MAIN(LoRa_Daemon_Test)

#include "tst_lora_daemon_test.moc"
