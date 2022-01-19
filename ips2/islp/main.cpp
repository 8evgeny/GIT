#include <QCoreApplication>
#include "islp.hpp"
#include "ipsnames.hpp"

int main(int argc, char *argv[])
{
  uint8_t c[] = "blalalalal";
  ISLP::stationResponse resp (&c[0], 3);
  TYPE_PRINT_TABLE(IPS::TypeID::PDO112P);
  std::cout << "Hi!.....\n";

  IPS::Stations::printTypeTable ();
//  QCoreApplication a (argc, argv);
//  return a.exec ();
}
