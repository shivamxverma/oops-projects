// Example: Logistics Application Handling Trucks & Ships
 
interface Transport {
  delivery() : void
}

class Truck implements Transport {
  delivery(): void {
      console.log('Truck has been created');
  }
}

class Aeroplane implements Transport {
  delivery(): void {
      console.log('AeroPlane has been created');
  }
}

abstract class Logistics {
  abstract createTransport(): Transport;

  planDelivery(): void {
    const transport = this.createTransport(); 
    transport.delivery(); 
  }
}

class RoadLogistics extends Logistics {
  createTransport(): Transport {
    return new Truck();
  }
}

class AeroLogistics extends Logistics {
  createTransport(): Transport {
    return new Aeroplane();
  }
}

function logisticsApp(logistics: Logistics) {
  logistics.planDelivery();
}

const roadTransport = new RoadLogistics();
const seaTransport = new AeroLogistics();
