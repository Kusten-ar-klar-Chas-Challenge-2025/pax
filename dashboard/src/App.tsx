import { useState } from "react";
import Header from "./components/Header";
import Footer from "./components/Footer";
import RoomCard from "./components/RoomCard";
import CreateRoomModal from "./components/CreateRoomModal";
import Sidebar from "./components/Sidebar";

type Room = {
  id: number;
  name: string;
  description?: string;
  available: boolean;
  air_quality: number;
  screen: boolean;
  floor: number;
  chairs: number;
  whiteboard: boolean;
  projector: boolean;
};

const App: React.FC = () => {
  const [rooms, setRooms] = useState<Room[]>([]);
  const [isModalOpen, setIsModalOpen] = useState(false);

  const API_BASE_URL = "http://localhost:13000";

  const handleDeleteRoom = async (id: number) => {
    if (!window.confirm("Är du säker på att du vill ta bort detta rum?")) return;
    try {
      const res = await fetch(`${API_BASE_URL}/api/room/${id}`, {
        method: "DELETE",
      });
      if (!res.ok) {
        throw new Error("Kunde inte ta bort rummet");
      }

      setRooms((prevRooms) => prevRooms.filter((room) => room.id !== id));
    } catch (error) {
      console.error("Fel deleting", error);
    }
  };

  const handleCreateRoom = (room: Room) => {
    setRooms((prev) => [...prev, room]);
  };

  return (
    <div className="min-h-screen flex">
      {/* Sidebar with button trigger */}
      <Sidebar onCreateRoomClick={() => setIsModalOpen(true)} />

      {/* Main content */}
      <div className="flex flex-col flex-grow bg-gray-300">
        <Header />

        <main className="flex-grow p-6">
          <div className="flex justify-between items-center mb-6">
            <h1 className="text-2xl font-bold">Dashboard</h1>
            <h2 className="text-xl font-semibold">Alla rum</h2>
          </div>

          {rooms.length === 0 ? (
            <p className="text-gray-500">Inga rum ännu. Skapa ett nytt rum!</p>
          ) : (
            <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
              {rooms.map((room) => (
                <RoomCard
                  key={room.id}
                  room={room}
                  onEdit={() => { }}
                  onDelete={handleDeleteRoom}
                />
              ))}
            </div>
          )}
        </main>

        <Footer />
      </div>

      {/* Modal för att skapa rum */}
      <CreateRoomModal
        isOpen={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        onCreate={handleCreateRoom}
      />
    </div>
  );
};

export default App;
