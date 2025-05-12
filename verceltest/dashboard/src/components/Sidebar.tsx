import { FC } from "react";

interface SidebarProps {
    onCreateRoomClick: () => void;
}

const Sidebar: FC<SidebarProps> = ({ onCreateRoomClick }) => {
    return (
        <aside className="bg-[#10302B] text-white w-64 h-screen p-6 flex flex-col justify-between">
            <div>
                {/* Centrera logotypen */}
                <div className="flex justify-center mb-5">
                    <img src="/pax.logo.png" alt="Pax logo" className="w-32" />
                </div>

                <nav>
                    <ul className="space-y-1 flex flex-col items-center">
                        <li><a href="#" className="block py-2 hover:underline">Dashboard</a></li>
                        <li><a href="#" className="block py-2 hover:underline">Statistik</a></li>
                        <li><a href="#" className="block py-2 hover:underline">Användare</a></li>
                    </ul>
                </nav>
            </div>

            <div>
                <button
                    onClick={onCreateRoomClick}
                    className="w-full bg-white text-blue-600 font-semibold py-2 px-4 rounded hover:bg-blue-100 transition"
                >
                    + Skapa nytt rum
                </button>
            </div>
        </aside>
    );
};

export default Sidebar;
