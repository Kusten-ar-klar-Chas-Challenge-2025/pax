import { FC, useState } from "react";

const Header: FC = () => {
    const [isOpen, setIsOpen] = useState(false);

    const toggleDropdown = () => {
        setIsOpen((prev) => !prev);
    };

    return (
        <header className="flex justify-end items-center px-6 py-4 bg-white relative">
            <div className="relative">
                <button
                    onClick={toggleDropdown}
                    className="flex items-center space-x-2 bg-gray-100 px-4 py-2 rounded-md text-sm font-medium text-gray-700 hover:bg-gray-200"
                >
                    <span>Admin</span>
                    <svg
                        className={`w-4 h-4 transition-transform ${isOpen ? "rotate-360" : ""}`}
                        fill="none"
                        stroke="currentColor"
                        strokeWidth="2"
                        viewBox="0 0 24 24"
                    >
                        <path strokeLinecap="round" strokeLinejoin="round" d="M19 9l-7 7-7-7" />
                    </svg>
                </button>

                {isOpen && (
                    <div className="absolute right-0 mt-2 w-40 bg-white border rounded-md shadow-lg z-10">
                        <button
                            className="block w-full text-left px-4 py-2 text-sm text-bold hover:bg-gray-100"
                            onClick={() => { }}
                        >
                            Logga ut
                        </button>
                    </div>
                )}
            </div>
        </header>
    );
};

export default Header;
